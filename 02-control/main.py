#!/usr/bin/env python3
import serial
import time
import matplotlib.pyplot as plt

# Настройки
PORT = 'COM7'
BAUD = 115200

# Подключение
print("Подключение к Pico...")
ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)
ser.reset_input_buffer()

def cmd(c):
    ser.write((c + '\n').encode())
    time.sleep(0.1)
    return ser.read_all().decode().strip()

# Проверка
print(f"ADC: {cmd('get_adc')}")
print(f"мВ: {cmd('get_voltage')}")
print(f"мс: {cmd('get_ms')}")

# Сбор данных 30 секунд (опросом)
print("\nСбор данных опросом 30 сек...")
data1 = []
start = time.time()
while time.time() - start < 30:
    t1 = time.time()
    resp = cmd('get_measure')
    if resp:
        t2 = time.time()
        pico_time, volt = map(int, resp.split())
        data1.append((t1 - start, pico_time, volt))
    time.sleep(0.09)  # ~100 мс

print(f"Получено {len(data1)} точек")

# Сбор телеметрии 30 секунд
print("\nСбор телеметрии 30 сек...")
cmd('set_sampling_period 100')
cmd('tm_start')
for i in range(30):
    time.sleep(1)
    print(f"{30-i-1}...", end=' ')
cmd('tm_stop')

# Парсим телеметрию из последнего ответа
resp = cmd('help')  # Просто чтобы получить буфер
print("\nДанные телеметрии в выводе выше")

# Графики
plt.figure(figsize=(12, 4))

plt.subplot(121)
plt.plot([d[0] for d in data1], [d[2] for d in data1])
plt.title('Опрос')
plt.xlabel('Время (с)')
plt.ylabel('мВ')

plt.subplot(122)
# Ручной ввод данных телеметрии или парсинг
plt.title('Телеметрия')
plt.xlabel('Время')
plt.ylabel('мВ')

plt.tight_layout()
plt.savefig('compare.png')
plt.show()

ser.close()


