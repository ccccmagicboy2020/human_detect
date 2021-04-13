#ifndef __MYADC_H
#define __MYADC_H

/*
 * If you remap the mapping between the channel and the pin with the function
 * ADC_ChannleRemap, define ADC_CH_REMAP as non-zero, otherwise define as 0.
 */
#define ADC_CH_REMAP                (1u)

/* ADC1 channel definition for this example. */
#define ADC1_SA_NORMAL_CHANNEL      (ADC1_CH6)
#define ADC1_AVG_CHANNEL   				  (ADC1_CH6)
#define ADC1_PGA_CHANNEL   				  (ADC1_CH6)
#define ADC1_SA_CHANNEL             (ADC1_SA_NORMAL_CHANNEL)
#define ADC1_SA_CHANNEL_COUNT       (1u)
/* ADC1 channel sampling time. */
#define ADC1_SA_CHANNEL_SAMPLE_TIME {25, }      //25*20ns = 500ns / 2M SPS
#define ADC1_CHANNEL                (ADC1_SA_CHANNEL)

#define ADC2_SA_NORMAL_CHANNEL      (ADC2_CH0|ADC2_CH1|ADC2_CH3|ADC2_CH4|ADC2_CH5)
#define ADC2_AVG_CHANNEL         		(ADC2_CH0|ADC2_CH1|ADC2_CH3|ADC2_CH4|ADC2_CH5)
#define ADC2_SA_CHANNEL             (ADC2_SA_NORMAL_CHANNEL)
#define ADC2_SA_CHANNEL_COUNT       (5u)
#define ADC2_SA_CHANNEL_SAMPLE_TIME {50, 50, 50, 50, 50, }			//50*20ns = 1000ns / 1M SPS
#define ADC2_CHANNEL                (ADC2_SA_CHANNEL)

/* PGA factor definitions. */
/* Choose a part from @ref en_adc_pga_factor_t. */		//P483
#define PGA_FACTOR_2X              	(2u)
#define PGA_FACTOR_3X              	(3u)
#define PGA_FACTOR                  (PGA_FACTOR_3X)

/* DMA definition for ADC1. */
#define ADC1_SA_DMA_UNIT            (M4_DMA1)
#define ADC1_SA_DMA_PWC             (PWC_FCG0_PERIPH_DMA1)
#define ADC1_SA_DMA_CH              (DmaCh0)
#define ADC1_SA_DMA_TRGSRC          (EVT_ADC1_EOCA)

#define ADC2_SA_DMA_UNIT            (M4_DMA2)
#define ADC2_SA_DMA_PWC             (PWC_FCG0_PERIPH_DMA2)
#define ADC2_SA_DMA_CH              (DmaCh0)
#define ADC2_SA_DMA_TRGSRC          (EVT_ADC2_EOCA)

extern void AdcConfig(void);

#endif

