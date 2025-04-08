# ĐỌC DỮ LIỆU ANALOG BẰNG STM32F429

STM32 có sẵn __3 bộ ADC 12-bit__ với tốc độ mâu 2,4 MSPS, tức là có khả năng lấy được 2.4 triệu mẫu giá trị trong 1 giây, mỗi giá trị là một số nguyên 12-bit trong phạm vi từ [0,4095].

![ASDC-DAC](./assets/adcdac.png)

Chương trình sau hỗ trợ đọc dữ liệu đã chuyển đổi Analog To Digital theo cả **3 phương pháp** (Chi tiết xem ở học phần Kiến trúc Máy tính):

1. [__Polling/Hỏi vòng__](#thực-hiện-với-phương-pháp-polling): đặc trưng với 2 vòng lặp: vòng lặp xác định trạng thái sẵn sàng của thiết bị, và vòng lặp đọc mảng dữ liệu.
2. [__Interrupt/Ngắt__](#thực-hiện-với-phương-pháp-interrupt): đặc trưng với 1 vòng lặp đọc mảng dữ liệu. Trong đó trạng thái sẵn sàng do thiết bị gửi tín hiệu ngắt báo hiệu.
3. __DMA/Truy cập bộ nhớ trực tiếp__: đặc trưng với 0 vòng lặp. Trong đó trạng thái sẵn sàng do thiết bị gửi tín hiệu ngắt báo hiệu và bộ điều khiển DMAC chịu trách nhiệm copy dữ liệu từ thiết bị/module IO trực tiếp vào bộ nhớ chính.

>Chú ý: trong chương trình này, dữ liệu từ cảm biến chỉ có 1 word dữ liệu đơn nên sẽ không nhìn thấy vòng lặp đọc mảng dữ liệu, mà đơn giản chỉ là 1 lệnh HAL_ADC_GetValue()

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

> Để đọc số liệu từ các dạng cảm biến này, chỉ cần cắm lại dây sang module cảm biến mới, mà không cần phải lập trình lại.

## Kết nối STM32F429 với module cảm biến

|STM32F429|Module cảm biến|
|:--:|:--:|
|3v3|Vcc|
|GND|GND|
|PA0 (đóng vai trò tín hiệu ngắt khi vượt ngưỡng)|D0|
|PA1|A0|

## Thực hiện với phương pháp Polling

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

## Thực hiện với phương pháp Interrupt

1. Như với polling
2. Như với polling
    - __2b Bổ sung__: Vẫn ở giao diện __ADC__ , trong mục __Configuration / NVIC Settings__, hãy __enable__ hộp chọn __ADC1, ADC2, and ADC3 global interrupts__.\
    ![Kích hoạt ngắt](./assets/enableint.png)
3. Như với polling
4. Như với polling
5. __Bỏ đoạn mã polling__ trong hàm main.c, và thay thế bằng xử lý handler trong sự kiện ngắt như sau.\
   Trong file __Src/stm32f4xx_it.c__, bổ sung vào hàm __ADC_IRQHandler()__
   ```C
    sensor_value = HAL_ADC_GetValue(&hadc1);
   ```

## Kết quả

   ![Ảnh 1](./assets/20250403_183009.jpg)
   ![Ảnh 2](./assets/20250403_183027.jpg)