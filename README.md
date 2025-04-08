# ĐỌC DỮ LIỆU ANALOG BẰNG STM32F429

   STM32 có sẵn __3 bộ ADC 12-bit__ với tốc độ mâu 2,4 MSPS, tức là có khả năng lấy được 2.4 triệu mẫu giá trị trong 1 giây, mỗi giá trị là một số nguyên 12-bit trong phạm vi từ [0,4095].

   ![ASDC-DAC](./assets/adcdac.png)

## Ứng dụng

Phù hợp để áp dụng cho các module cảm biến có chân trả về __A0__ (chữ A có ý nghĩa là Analog). Ví dụ:

- Biến trở.\
  ![biến trở](./assets/bientro.png)
- Module MQ-03 - cảm biến nồng độ cồn.\
  ![mq3](./assets/mq3.png)
- Module MQ-08 - cảm biến khi gas dễ cháy.\
  ![mq9](./assets/mq9.png) \
- Module cường độ sáng.\
  ![cuongdosang](./assets/cuongdosang.png)
- Module âm thanh.\
  ![alt text](./assets/amthanh.png)

> Để đọc số liệu từ các dạng cảm biến này, chỉ cần cắm lại dây sang module cảm biến mới, mà không cần phải lập trình lại.

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

4. Trong file __main.c__, khởi tạo biến để chứa giá trị analog đọc được.

    ```C
      uint16_t sensor_value;
    ```

5. Trong file __main.c__, thực hiện đọc số liệu với 3 bước:
    1. Yêu cầu __ADC__ chuyển đổi
    2. Chờ __ADC__ chuyển đổi xong
    3. Lấy giá trị từ __ADC__.
  
    ```C
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 20);
    sensor_value = HAL_ADC_GetValue(&hadc1);
    ```

## Kết quả
   [Video demo![alt text](./assets/video_demo.png)](https://youtube.com/shorts/wBLDlglATeg)
   ![Ảnh 1](./assets/20250403_183009.jpg)
   ![Ảnh 2](./assets/20250403_183027.jpg)