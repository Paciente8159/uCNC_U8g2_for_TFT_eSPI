////////////////////////////////////////////////////
//       TFT_eSPI generic driver functions        //
////////////////////////////////////////////////////

// This is a generic driver for Arduino boards, it supports SPI interface displays
// 8-bit parallel interface to TFT is not supported for generic processors

#ifndef _TFT_eSPI_UCNCH_
#define _TFT_eSPI_UCNCH_

// Processor ID reported by getSetup()
#define PROCESSOR_ID 0x0000

// Include processor specific header
// None

// Processor specific code used by SPI bus transaction startWrite and endWrite functions
#define SET_BUS_WRITE_MODE // Not used
#define SET_BUS_READ_MODE	 // Not used

// Code to check if DMA is busy, used by SPI bus transaction startWrite and endWrite functions
#define DMA_BUSY_CHECK // Not used so leave blank

// To be safe, SUPPORT_TRANSACTIONS is assumed mandatory
#if !defined(SUPPORT_TRANSACTIONS)
#define SUPPORT_TRANSACTIONS
#endif

// Initialise processor specific SPI functions, used by init()
#define INIT_TFT_DATA_BUS

// If smooth fonts are enabled the filing system may need to be loaded
#ifdef SMOOTH_FONT
// Call up the filing system for the anti-aliased fonts
// #define FS_NO_GLOBALS
// #include <FS.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the DC (TFT Data/Command or Register Select (RS))pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_DC
#define DC_C // No macro allocated so it generates no code
#define DC_D // No macro allocated so it generates no code
#else
#if ASSERT_PIN_IO(TFT_DC)
#define DC_C io_clear_output(TFT_DC)
#define DC_D io_set_output(TFT_DC)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the CS (TFT chip select) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_CS
#define CS_L // No macro allocated so it generates no code
#define CS_H // No macro allocated so it generates no code
#else
#if ASSERT_PIN_IO(TFT_CS)
#define CS_L io_clear_output(TFT_CS)
#define CS_H io_set_output(TFT_CS)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Make sure TFT_RD is defined if not used to avoid an error message
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_RD
#define TFT_RD -1
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the WR (TFT Write) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifdef TFT_WR
#if ASSERT_PIN_IO(TFT_WR)
#define WR_L io_clear_output(TFT_WR)
#define WR_H io_set_output(TFT_WR)
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the touch screen chip select pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if !defined TOUCH_CS || (TOUCH_CS < 0)
#define T_CS_L // No macro allocated so it generates no code
// #define T_CS_H // No macro allocated so it generates no code
#else
#if ASSERT_PIN_IO(TOUCH_CS)
#define T_CS_L io_clear_output(TOUCH_CS)
#define T_CS_H io_set_output(TOUCH_CS)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Make sure TFT_MISO is defined if not used to avoid an error message
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_MISO
#define TFT_MISO -1
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros to write commands/pixel colour data to a SPI ILI948x TFT
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_UCNC_SPI
#define TFT_UCNC_SPI 0
#endif

#if defined(SPI_18BIT_DRIVER) // SPI 18-bit colour

// Write 8 bits to TFT
#define tft_Write_8(C) softspi_xmit(TFT_UCNC_SPI, C)

// Convert 16-bit colour to 18-bit and write in 3 bytes
#define tft_Write_16(C)                            \
	softspi_xmit(TFT_UCNC_SPI, ((C) & 0xF800) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, ((C) & 0x07E0) >> 3); \
	softspi_xmit(TFT_UCNC_SPI, ((C) & 0x001F) << 3)

// Convert swapped byte 16-bit colour to 18-bit and write in 3 bytes
#define tft_Write_16S(C)                                                \
	softspi_xmit(TFT_UCNC_SPI, (C) & 0xF8);                               \
	softspi_xmit(TFT_UCNC_SPI, ((C) & 0xE000) >> 11 | ((C) & 0x07) << 5); \
	softspi_xmit(TFT_UCNC_SPI, ((C) & 0x1F00) >> 5)
// Write 32 bits to TFT
#define tft_Write_32(C)                           \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 24); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 16); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 8);  \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C))

// Write two address coordinates
#define tft_Write_32C(C, D)                      \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C));      \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(D) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(D))

// Write same value twice
#define tft_Write_32D(C)                         \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C));      \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)(C))

////////////////////////////////////////////////////////////////////////////////////////
// Macros to write commands/pixel colour data to other displays
////////////////////////////////////////////////////////////////////////////////////////
#else
#if defined(RPI_DISPLAY_TYPE) // RPi TFT type always needs 16-bit transfers
#define tft_Write_8(C)           \
	softspi_xmit(TFT_UCNC_SPI, C); \
	softspi_xmit(TFT_UCNC_SPI, C)
#define tft_Write_16(C)                            \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)((C) >> 8)); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)((C) >> 0))
#define tft_Write_16S(C)                           \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)((C) >> 0)); \
	softspi_xmit(TFT_UCNC_SPI, (uint8_t)((C) >> 8))

#define tft_Write_32(C)                \
	tft_Write_16((uint16_t)((C) >> 16)); \
	tft_Write_16((uint16_t)((C) >> 0))

#define tft_Write_32C(C, D)             \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 0); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (D) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (D) >> 0)

#define tft_Write_32D(C)                \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 0); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 8); \
	softspi_xmit(TFT_UCNC_SPI, 0);        \
	softspi_xmit(TFT_UCNC_SPI, (C) >> 0)

#else
#ifdef __AVR__ // AVR processors do not have 16-bit transfer
#define tft_Write_8(C) softspi_xmit(TFT_UCNC_SPI, C)
#define tft_Write_16(C)             \
	tft_Write_8((uint8_t)((C) >> 8)); \
	tft_Write_8((uint8_t)((C) >> 0))
#define tft_Write_16S(C)            \
	tft_Write_8((uint8_t)((C) >> 0)); \
	tft_Write_8((uint8_t)((C) >> 8))
#else
#define tft_Write_8(C) softspi_xmit(TFT_UCNC_SPI, C)
#define tft_Write_16(C)             \
	tft_Write_8((uint8_t)((C) >> 8)); \
	tft_Write_8((uint8_t)((C) >> 0))
#define tft_Write_16S(C)            \
	tft_Write_8((uint8_t)((C) >> 0)); \
	tft_Write_8((uint8_t)((C) >> 8))
#endif // AVR

#define tft_Write_32(C)                \
	tft_Write_16((uint16_t)((C) >> 16)); \
	tft_Write_16((uint16_t)((C) >> 0))

#define tft_Write_32C(C, D)    \
	tft_Write_16((uint16_t)(C)); \
	tft_Write_16((uint16_t)(D))

#define tft_Write_32D(C)       \
	tft_Write_16((uint16_t)(C)); \
	tft_Write_16((uint16_t)(C))
#endif // RPI_DISPLAY_TYPE
#endif

#ifndef tft_Write_16N
#define tft_Write_16N tft_Write_16
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros to read from display using SPI or software SPI
////////////////////////////////////////////////////////////////////////////////////////
#if defined(TFT_SDA_READ)
// Use a bit banged function call for STM32 and bi-directional SDA pin
#define TFT_eSPI_ENABLE_8_BIT_READ // Enable tft_Read_8(void);
#define SCLK_L io_clear_output(TFT_SCLK)
#define SCLK_H io_set_output(TFT_SCLK)
#else
// Use a SPI read transfer
#define tft_Read_8() softspi_xmit(TFT_UCNC_SPI, 0)
#endif

#endif // Header end
