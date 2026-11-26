# UART ISP device protocol - RL78/G22

This is the contract for the independent RL78_G22_ISP_Boot_loader_UART, RL78_G22_ISP_Boot_loader_UART_app and RL78_G22_UART_Transfer projects. [Tool operation](https://released.github.io/slide_simple_renesas_isp_tool/uart.html) owns wiring and UI steps. The device UART0 Log is a separate text connection.

## Transport and identity

| Item | Value |
| --- | --- |
| Update link | UART1 TX / RX P00 / P01, 115200 8-N-1 |
| Contract / protocol ID | 1 / 0x55 |
| Image magic | ISP plus NUL, bytes 49 53 50 00 |
| Metadata | App+0x100, 256 bytes; core128 and reserved128 FF |
| Metadata protocol word | 0x01010055 |
| Policy word at metadata0x64 | 0x04012003 (policy3,32 bytes,DEVICE_APP,UART) |
| Policy baud at0x70 | 115200 |
| Version policy at0x74 | 0=any version;1=same/newer from valid installed App |
| Link words0x30..0x3F | 0,0,0,0 |

Diagnostic availability comes from live INFO;Boot does not advertise bit6.
Snapshot fields are defined in the [serial diagnostic reference](https://released.github.io/slide_boot_loader_for_RL78/serial-device.html#serial-uart-diagnostics).
Protocol CLEAR_STATS does not reset the board's diagnostic counters.

## Framed control bytes

| Offset | Bytes | Meaning |
| --- | --- | --- |
| 0 | 2 | 49 53 frame marker |
| 2 | 1 | Framing version1 |
| 3 | 1 | 0=request,1=response |
| 4 | 2 | Sequence LE16,1..65535 |
| 6 | 1 | Command |
| 7 | 1 | Status;request0 |
| 8 | 2 | Payload length LE16,0..96 |
| 10 | 0..96 | Payload |
| 10+length | 4 | CRC32 of header/payload,LE32 |

CRC32/ISO-HDLC uses reflected polynomial EDB88320, initial/final XOR FFFFFFFF; 123456789 gives CBF43926. Maximum frame110 bytes; partial framed input expires after200ms. Equal sequence replay requires identical payload. Conflicting duplicates are rejected.

## Commands and update boundaries

| Command | Payload / result |
| --- | --- |
| 01 INFO | 88-byte live MCU/profile/geometry/installed identity/capabilities |
| 02 ENTER | App replies completely before resetting into Boot;Boot ACK |
| 03 METADATA | LE16 offset plus4..64 bytes;ordered stage256 before START |
| 04 START /0E FORCE_START | LE32 full image size plusLE32 stored final CRC;validate before erase |
| 05 WRITE /06 END | WRITE=LE32 offset plus4..64 bytes;END validates whole image before reset |
| 07 ABORT | Clear session/staging;does not restore erased contents |
| 08 READ_METADATA | Valid installed App;LE16 offset plusU8 count1..64 |
| 09 VERIFY | Boot read-only installed-image validation |
| 20 PING /21 ECHO | App/Transfer only;0..96-byte echo |
| 22 STATS /23 CLEAR_STATS | App/Transfer only;fourLE32 counters:requests,replays,errors,written |
| 25 DIAGNOSTICS | Optional App/Transfer INFO bit6;24-byte UART errors/drops/TX timeout/queue snapshot |



Framed ISP requests stage metadata and check START before ordered 64-byte writes. END checks complete length, stored trailer, full CRC32, metadata and App vectors before reset.

Boot checks metadata, MCU/partition/IRQ/minimum-Boot contract, valid-installed-App policy, range, stored trailer and full CRC before launch. FORCE skips ordering only for its transaction;no persistent unlock/floor. Boot idle30s returns to an already valid App only before an active update. App ISR authoring uses app_irq_handlers.h,AA/55 ownership and generated full tables.

Build/package/model results are offline evidence;board electrical/IRQ/Flash/reset/power acceptance is separate.
