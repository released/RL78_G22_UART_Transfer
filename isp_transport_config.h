/* Project/API reading notes: One selected device update interface.
 * Baud/address/protocol values must match the board backend, paired Boot/App image contract and
 * Tool selection.
 * The independent diagnostic Log UART is not the binary ISP data channel.
 */
#ifndef ISP_TRANSPORT_CONFIG_H
#define ISP_TRANSPORT_CONFIG_H
/* Device UART1 uses the requested8-N-1 baud below (default115200).
 * The board derives a checked SDR from its selected SAU source clock.
 * Rates outside that divisor range require a reviewed SPS/source change.
 * UART0 is diagnostic-only and keeps its independent115200 setting.
 * Boot and App use the same setting; the Tool must match this value.
 * RX vector 0x26 comes from this MCU's iodefine.h INTSR1 definition.
 */
#define ISP_UART_BAUD (115200UL)
#define ISP_UART_RX_VECTOR (0x26U)
#define ISP_INTERBYTE_TIMEOUT_MS (200U)
#define ISP_BOOT_IDLE_TIMEOUT_MS (30000U)
#define ISP_BOOT_PROTOCOL_ID (0x55U)
#define ISP_BOOT_PROTOCOL_VERSION (1U)
/* Reserved link words retain the common 128-byte metadata core positions. */
#define ISP_LINK_MODE (0U)
#define ISP_LINK_ADDRESS_KIND (0U)
#define ISP_LINK_PARAMETER_0 (0UL)
#define ISP_LINK_PARAMETER_1 (0UL)
#define ISP_LINK_PARAMETER_2 (0UL)
#define ISP_LINK_PARAMETER_3 (0UL)
#define ISP_BOOT_DEBUG_UART (0U)
#endif
