# EColor

EColor - это утилита для мгновенного определения цвета пикселя на экране компьютера. Данная утилита находится в режиме ожидания в системном лотке до появления события нажатия запрограммированного сочетания клавиш на клавиатуре. При возникновении данного события утилита определяет цвет пикселя в месте положения курсора мыши.

# Использование под linux deb based

```
sudo apt-get install qtx11extras5-dev-tools
qmake ecolor.pro
make
cd bin/
chmod +x EColor
./EColor
```

# Для Linux Ubuntu 16.04
```
sudo apt-get install libqt5x11extras5-dev
qmake ecolor.pro
make
cd bin/
chmod +x EColor
./EColor
```

## Command line
* -popup-show - показывать окно выбора цвета при запуске
* -popup-terminate - закрывать программу после закрытия окна выбора цвета
* -popup-gradation-open - показать градации цвета при запуске
