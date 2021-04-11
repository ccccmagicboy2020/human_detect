#ifndef __MYADC_H
#define __MYADC_H

/*
 * If you remap the mapping between the channel and the pin with the function
 * ADC_ChannleRemap, define ADC_CH_REMAP as non-zero, otherwise define as 0.
 */
#define ADC_CH_REMAP                (0u)

/* ADC1 channel definition for this example. */
#define ADC1_SA_NORMAL_CHANNEL      (ADC1_CH6|ADC1_CH9)
#define ADC1_AVG_CHANNEL   				  (ADC1_CH6)
#define ADC1_PGA_CHANNEL   				  (ADC1_CH6)
#define ADC1_SA_CHANNEL             (ADC1_SA_NORMAL_CHANNEL)
#define ADC1_SA_CHANNEL_COUNT       (2u)

#define ADC1_CHANNEL                (ADC1_SA_CHANNEL)

/* ADC1 channel sampling time. */
#define ADC1_SA_CHANNEL_SAMPLE_TIME {25, 25}      //25*20ns = 500ns / 2M SPS

/* PGA factor definitions. */
/* Choose a part from @ref en_adc_pga_factor_t. */
#define PGA_FACTOR_2X              (2u)
#define PGA_FACTOR_3X              (3u)
#define PGA_FACTOR_4X								(4u)
#define PGA_FACTOR                  (PGA_FACTOR_3X)

/* DMA definition for ADC1. */
#define ADC1_SA_DMA_UNIT            (M4_DMA1)
#define ADC1_SA_DMA_PWC             (PWC_FCG0_PERIPH_DMA1)
#define ADC1_SA_DMA_CH              (DmaCh0)
#define ADC1_SA_DMA_TRGSRC          (EVT_ADC1_EOCA)



extern void AdcConfig(void);

#endif

