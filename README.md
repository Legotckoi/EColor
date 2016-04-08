# EColor
![functional](https://raw.githubusercontent.com/ilopX/EColor/tmp/tmp/images/v1.6-func.gif)

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
## Command line
* -popup-show - показывать окно выбора цвета при запуске
* -popup-terminate - закрывать программу после закрытия окна выбора цвета
* -popup-gradation-open - показать градации цвета при запуске