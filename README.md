# ĐỌC DỮ LIỆU ANALOG BẰNG STM32F429

   STM32 có sẵn __3 bộ ADC 12-bit__ với tốc độ mâu 2,4 MSPS, tức là có khả năng lấy được 2.4 triệu mẫu giá trị trong 1 giây, mỗi giá trị là một số nguyên 12-bit trong phạm vi từ [0,4095].

   ![ASDC-DAC](./assets/adcdac.png)

## Ứng dụng

Phù hợp để áp dụng cho các module cảm biến có chân trả về __A0__ (chữ A có ý nghĩa là Analog). Ví dụ:

- Module MQ-03 - cảm biến nồng độ cồn.\
  ![mq3](./assets/mq3.png)
- Module MQ-08 - cảm biến khi gas dễ cháy.\
  ![mq9](./assets/mq9.png) \
- Module cường độ sáng.\
  ![cuongdosang](./assets/cuongdosang.png)
- Module âm thanh.\
  ![alt text](./assets/amthanh.png)

## Kết nối STM32F429 với module cảm biến

|STM32F429|Module cảm biến|
|:--:|:--:|
|3v3|Vcc|
|GND|GND|
|(bỏ qua)|D0|
|PA1|A0|

## Thực hiện

1. Tạo dự án mới với __MCU__ là __STM32F429zIT6__\
  ![alt text](./assets/newproject_with_mcu.png)
2. Mở file __.ioc__, Chọn bộ __ADC__ sẽ sử dụng (ADC1, ADC2, hay ADC3). Ứng với bộ __ADC__ đó, chọn kênh đầu vào là __IN nào__ trong số 16 nguồn vào.\
  ![alt text](./assets/selectadc.png)
3. Vẫn trong file __.ioc__, bổ sung thêm __UART1__ để truyền số liệu thu được về máy tính, phục vụ để debug.\
  *Lưu ý rằng: __UART1__ trên STM32 sẽ giao tiếp với máy tính qua chính cổng USB mini dùng để nạp chương trình, nên sẽ không cần cắm dây bổ sung.*\
  ![alt text](./assets/caidatuart1.png)