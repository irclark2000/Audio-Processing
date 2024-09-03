/*
 * audio_wm8960.c
 *
 *  Created on: Aug 13, 2024
 *      Author: isaac
 */
#include "audio_wm8960.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void setupDMA (void);
void setupSAI (void);
#if defined(__cplusplus)
extern "C" {
#endif
static void EDMA_RX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
static void EDMA_TX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
#if defined(__cplusplus)
}
#endif // __cplusplus
static void AUDIO_StartTXPingPongBuffer(void *buffer1, uint32_t bufferSize);
static void AUDIO_StartRXPingPongBuffer(void *buffer1, uint32_t bufferSize);
extern void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync);
/*******************************************************************************
 * Variables
 *******************************************************************************/
wm8960_config_t wm8960Config = {
		.i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
		.route     = kWM8960_RoutePlaybackandRecord,
		.leftInputSource  = kWM8960_InputDifferentialMicInput3,
		.rightInputSource = kWM8960_InputDifferentialMicInput2,
		.playSource       = kWM8960_PlaySourceDAC,
		.slaveAddress     = WM8960_I2C_ADDR,
		.bus              = kWM8960_BusI2S,
		//    .format = {.mclk_HZ = 6144000U, .sampleRate = DEMO_AUDIO_SAMPLE_RATE, .bitWidth = kWM8960_AudioBitWidth16bit},
		.format = {.mclk_HZ = DEMO_WM8960_MCLK, .sampleRate = DEMO_AUDIO_SAMPLE_RATE, .bitWidth = kWM8960_AudioBitWidth16bit},
		.master_slave = false,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
		.loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
		.postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
		.numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
		.denominator = 100, /* 30 bit denominator of fractional loop divider */
};

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_bufferIn[2*HALF_BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_bufferOut[2*HALF_BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static edma_tcd_t s_emdaTcd[4], 32);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
//static uint32_t tx_index = 0U, rx_index = 0U;
//volatile uint32_t emptyBlock = BUFFER_NUMBER;
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;
void * g_bufIn;
void * g_bufOut;
volatile bool g_Transfer_Done = false;
static edma_handle_t s_dmaTxHandle = {0};
static edma_handle_t s_dmaRxHandle = {0};
static sai_transceiver_t s_saiConfig;
static codec_handle_t s_codecHandle;
void audio_startup(void) {
	CLOCK_InitAudioPll(&audioPllConfig);
}
void WM8960_InitPins() {
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_SAI1_MCLK, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_SAI1_RX_DATA00, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_SAI1_TX_DATA00, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_SAI1_TX_BCLK, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_SAI1_TX_SYNC, 1U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL, 0xD8B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA, 0xD8B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_SAI1_MCLK, 0x10B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_SAI1_RX_DATA00, 0x10B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_SAI1_TX_DATA00, 0x10B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_SAI1_TX_BCLK, 0x10B0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_SAI1_TX_SYNC, 0x10B0U);
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
	if (enable)
	{
		IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
	}
	else
	{
		IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
	}
}
void DelayMS(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }
}

static void EDMA_RX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	if (!transferDone) {   // half complete transfer
		g_bufIn = s_bufferIn;
		g_bufOut = s_bufferOut;
	}
	else {                 // full complete transfer
		g_bufIn = &s_bufferIn[HALF_BUFFER_SIZE];
		g_bufOut = &s_bufferOut[HALF_BUFFER_SIZE];
	}
	g_Transfer_Done = true;  // process enabling flag
    __NOP();
}

static void EDMA_TX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    __NOP();
}

/****
 * Audio setup section
 */

void setup_SAI_EDMA() {

	CLOCK_InitAudioPll(&audioPllConfig);
	/*Clock setting for LPI2C*/
	CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

	/*Clock setting for SAI1*/
	CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
	CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
	CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

	/*Enable MCLK clock*/
	BOARD_EnableSaiMclkOutput(true);

	// Setup DMA and SAI
	setupDMA();
	setupSAI();

	/* master clock configurations */
	BOARD_MASTER_CLOCK_CONFIG();

	/* Use default setting to init codec */
	if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
	{
		assert(false);
	}
	if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
			DEMO_CODEC_VOLUME) != kStatus_Success)
	{
		assert(false);
	}
}
void setupSAI () {
    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&s_saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    s_saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    s_saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TxSetConfig(DEMO_SAI, &s_saiConfig);
    s_saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    s_saiConfig.syncMode    = DEMO_SAI_RX_SYNC_MODE;
    SAI_RxSetConfig(DEMO_SAI, &s_saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&s_codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    /* delay for codec output stable */
    DelayMS(DEMO_CODEC_INIT_DELAY_MS);

    AUDIO_StartRXPingPongBuffer(s_bufferIn, HALF_BUFFER_SIZE*2);
    AUDIO_StartTXPingPongBuffer(s_bufferOut, HALF_BUFFER_SIZE*2);
}
void setupDMA () {
	edma_config_t dmaConfig = {0};
    /* Init DMAMUX */
    //DMAMUX_Init(DEMO_DMAMUX); handled elsewhere so it is not called twice
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_AUDIO_TX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_AUDIO_TX_EDMA_CHANNEL);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_AUDIO_RX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_AUDIO_RX_EDMA_CHANNEL);

    PRINTF("SAI EDMA ping pong buffer example started!\n\r");
    PRINTF("SAI Clock: %d\n\r", (int) DEMO_SAI_CLK_FREQ);
    PRINTF("I2C Clock: %d\n\r", (int) DEMO_I2C_CLK_FREQ);
    PRINTF("Audio PLL Clock: %d\n\r", (int) CLOCK_GetFreq(kCLOCK_AudioPllClk));

    memset(s_bufferIn, 0, HALF_BUFFER_SIZE*2);
    memset(s_bufferOut, 0, HALF_BUFFER_SIZE*2);

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    //EDMA_GetDefaultConfig(&dmaConfig);
    //EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&s_dmaTxHandle, DEMO_DMA, DEMO_AUDIO_TX_EDMA_CHANNEL);
    EDMA_CreateHandle(&s_dmaRxHandle, DEMO_DMA, DEMO_AUDIO_RX_EDMA_CHANNEL);
    EDMA_SetCallback(&s_dmaTxHandle, EDMA_TX_Callback, NULL);
    EDMA_SetCallback(&s_dmaRxHandle, EDMA_RX_Callback, NULL);
    EDMA_ResetChannel(DEMO_DMA, DEMO_AUDIO_TX_EDMA_CHANNEL);
    EDMA_ResetChannel(DEMO_DMA, DEMO_AUDIO_RX_EDMA_CHANNEL);
}
static void AUDIO_StartRXPingPongBuffer(void *buffer, uint32_t bufferSize)
{
    edma_transfer_config_t transferConfig = {0};
    uint32_t srcAddr                      = SAI_RxGetDataRegisterAddress(DEMO_SAI, DEMO_SAI_CHANNEL);

    /* Configure and submit transfer structure 1 */
    EDMA_PrepareTransfer(&transferConfig, (void *)srcAddr, DEMO_AUDIO_BIT_WIDTH / 8U, buffer,
                         DEMO_AUDIO_BIT_WIDTH / 8U,
                         (FSL_FEATURE_SAI_FIFO_COUNTn(0) - s_saiConfig.fifo.fifoWatermark) * (DEMO_AUDIO_BIT_WIDTH / 8U),
                         bufferSize, kEDMA_PeripheralToMemory);

    EDMA_TcdSetTransferConfig(&s_emdaTcd[2], &transferConfig, &s_emdaTcd[3]);
    EDMA_PrepareTransfer(&transferConfig, (void *)srcAddr, DEMO_AUDIO_BIT_WIDTH / 8U, buffer,
                         DEMO_AUDIO_BIT_WIDTH / 8U,
                         (FSL_FEATURE_SAI_FIFO_COUNTn(0) - s_saiConfig.fifo.fifoWatermark) * (DEMO_AUDIO_BIT_WIDTH / 8U),
                         bufferSize, kEDMA_PeripheralToMemory);
    EDMA_TcdSetTransferConfig(&s_emdaTcd[3], &transferConfig, &s_emdaTcd[2]);
    EDMA_TcdEnableInterrupts(&s_emdaTcd[2], kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
    EDMA_TcdEnableInterrupts(&s_emdaTcd[3], kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
    EDMA_InstallTCD(DEMO_DMA, s_dmaRxHandle.channel, &s_emdaTcd[2]);
    EDMA_StartTransfer(&s_dmaRxHandle);
    /* Enable DMA enable bit */
    SAI_RxEnableDMA(DEMO_SAI, kSAI_FIFORequestDMAEnable, true);
    /* Enable SAI Tx clock */
    SAI_RxEnable(DEMO_SAI, true);
    /* Enable the channel FIFO */
    SAI_RxSetChannelFIFOMask(DEMO_SAI, 1U << DEMO_SAI_CHANNEL);
}

static void AUDIO_StartTXPingPongBuffer(void *buffer, uint32_t bufferSize)
{
    edma_transfer_config_t transferConfig = {0};
    uint32_t destAddr                     = SAI_TxGetDataRegisterAddress(DEMO_SAI, DEMO_SAI_CHANNEL);

    /* Configure and submit transfer structure 1 */
    EDMA_PrepareTransfer(&transferConfig, buffer, DEMO_AUDIO_BIT_WIDTH / 8U, (void *)destAddr,
                         DEMO_AUDIO_BIT_WIDTH / 8U,
                         (FSL_FEATURE_SAI_FIFO_COUNTn(0) - s_saiConfig.fifo.fifoWatermark) * (DEMO_AUDIO_BIT_WIDTH / 8U),
                         bufferSize, kEDMA_MemoryToPeripheral);

    EDMA_TcdSetTransferConfig(&s_emdaTcd[0], &transferConfig, &s_emdaTcd[1]);
    EDMA_PrepareTransfer(&transferConfig, buffer, DEMO_AUDIO_BIT_WIDTH / 8U, (void *)destAddr,
                         DEMO_AUDIO_BIT_WIDTH / 8U,
                         (FSL_FEATURE_SAI_FIFO_COUNTn(0) - s_saiConfig.fifo.fifoWatermark) * (DEMO_AUDIO_BIT_WIDTH / 8U),
                         bufferSize, kEDMA_MemoryToPeripheral);
    EDMA_TcdSetTransferConfig(&s_emdaTcd[1], &transferConfig, &s_emdaTcd[0]);
    EDMA_TcdEnableInterrupts(&s_emdaTcd[0], kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
    EDMA_TcdEnableInterrupts(&s_emdaTcd[1], kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
    EDMA_InstallTCD(DEMO_DMA, s_dmaTxHandle.channel, &s_emdaTcd[0]);
    EDMA_StartTransfer(&s_dmaTxHandle);
    /* Enable DMA enable bit */
    SAI_TxEnableDMA(DEMO_SAI, kSAI_FIFORequestDMAEnable, true);
    /* Enable SAI Tx clock */
    SAI_TxEnable(DEMO_SAI, true);
    /* Enable the channel FIFO */
    SAI_TxSetChannelFIFOMask(DEMO_SAI, 1U << DEMO_SAI_CHANNEL);
}
