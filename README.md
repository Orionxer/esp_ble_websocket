# ble_websocket

基于 ESP-IDF v5.5.3 的 ESP32-C5 示例工程，当前已实现：

- Wi-Fi STA 联网
- `ws` / `wss` WebSocket 通信
- NimBLE GATT Server
- BLE 控制 `GPIO27` LED
- BLE 收到开关灯请求后，将 `led turned on!` / `led turned off!` 通过 WebSocket 作为普通文本发送到服务端

## 1. 下载项目

```bash
git clone <your-repo-url>
cd ble_websocket
```

如果你是直接拷贝工程目录，也可以跳过 `git clone`，直接进入项目根目录。

## 2. 环境准备

本工程基于：

- ESP-IDF `v5.5.3`
- 目标芯片：`ESP32-C5`

激活环境：

```bash
source ~/.espressif/tools/activate_idf_v5.5.3.sh
```

## 3. 配置项目

当前默认配置位于 `sdkconfig`，主要包括：

- Wi-Fi SSID：`NRadio`
- WebSocket URI：`wss://gogo.uno:1886`
- LED GPIO：`27`

如需修改，可执行：

```bash
idf.py menuconfig
```

重点配置位置：

- `BLE WebSocket Configuration`
- `BLE Configuration`

## 4. 编译、烧录、监控

编译：

```bash
idf.py build
```

烧录：

```bash
idf.py -b 6000000 flash
```

查看串口日志：

```bash
idf.py monitor
```

## 5. 运行说明

设备启动后会：

1. 连接已配置的 Wi-Fi
2. 启动 WebSocket 客户端
3. 启动 BLE GATT Server 并开始广播
4. 等待 BLE Central 连接并写入 LED 特征值

当 BLE Central 写入：

- `0x01`：点亮 LED，并向 WebSocket 服务端发送 `led turned on!`
- `0x00`：熄灭 LED，并向 WebSocket 服务端发送 `led turned off!`

## 6. 测试验证

建议按下面流程验证：

1. 上电后观察串口，确认 Wi-Fi、WebSocket、BLE 都已启动
2. 用手机或 BLE 调试工具连接设备 `NimBLE_GATT`
3. 向 LED 特征值写入 `01`
4. 观察：
   - `GPIO27` 对应 LED 点亮
   - 串口打印 `led turned on!`
   - WebSocket 服务端收到文本 `led turned on!`
5. 再向 LED 特征值写入 `00`
6. 观察：
   - `GPIO27` 对应 LED 熄灭
   - 串口打印 `led turned off!`
   - WebSocket 服务端收到文本 `led turned off!`

## 7. 串口关键日志示例

```text
I (...) Main: WebSocket client started, target=wss://gogo.uno:1886
I (...) NimBLE_GATT_Server: advertising started!
I (...) Main: WebSocket connected over WSS: wss://gogo.uno:1886
I (...) NimBLE_GATT_Server: led turned on!
I (...) Main: Sent text command: led turned on!
```

## 8. 当前状态

当前版本已验证：

- BLE 与 WebSocket 可并行工作
- BLE Peripheral 可接收 Central 请求
- LED 控制走 `RMT + GPIO27`
- BLE 侧灯控结果可转发到 WebSocket 服务端
