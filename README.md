# 8051 AT89S52 Examples

Code ví dụ lập trình vi điều khiển **8051 AT89S52** đi kèm các bài viết trên blog [khuenguyencreator.com](https://khuenguyencreator.com).

Toàn bộ code viết cho Keil C51, thạch anh 12MHz (riêng bài UART dùng 11.0592MHz), mô phỏng được trên Proteus.

## Danh sách bài

| Bài | Thư mục | Nội dung |
|-----|---------|----------|
| 4 | `bai_4_gpio_led_nut_nhan` | GPIO bật tắt LED bằng nút nhấn, chống dội phím (mỗi file `.c` là 1 ví dụ riêng) |
| 5 | `bai_5_led_trai_tim` | LED trái tim 32 bóng với các hiệu ứng |
| 6 | `bai_6_quet_led_7_thanh` | LED 7 thanh: đếm 0-9, quét 2 LED 00-99, quét n LED |
| 7 | `bai_7_lcd1602` | LCD1602 chế độ 8 bit |
| 8 | `bai_8_timer` | Timer: ngắt định kỳ và delay chính xác |
| 9 | `bai_9_ngat_ngoai_exti` | Ngắt ngoài đếm xung |
| 10 | `bai_10_i2c_ds3231` | I2C mềm đọc ghi DS3231 |
| 11 | `bai_11_spi_flash_25q80` | SPI mềm với Flash W25Q80 |
| 12 | `bai_12_pwm` | PWM mềm bằng ngắt Timer0 |
| 13 | `bai_13_uart` | UART giao tiếp máy tính |
| 14 | `bai_14_1wire_ds18b20` | 1-Wire đọc nhiệt độ DS18B20 |
| 15 | `bai_15_dong_ho_led7` | Dự án đồng hồ LED 7 thanh 4 số |

Với thư mục có nhiều file `.c`, mỗi file là một chương trình độc lập có hàm `main` riêng: chỉ add 1 file vào project Keil mỗi lần.

Code 8051 cho chip Nuvoton: [8051-n76e885-examples](https://github.com/khuenguyencreator/8051-n76e885-examples).

## Liên kết

- 📖 Bài viết hướng dẫn chi tiết: [khuenguyencreator.com](https://khuenguyencreator.com)
- 📚 Các repo khác: [github.com/khuenguyencreator](https://github.com/khuenguyencreator)
