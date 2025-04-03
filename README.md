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

1. Tạo dự án mới với __MCU__ là __STM32F429zIT6__
    ![alt text](./assets/newproject_with_mcu.png)
