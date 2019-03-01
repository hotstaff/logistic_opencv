# logistic_opencv

Plot very large logistic map using opencv.
For example, 26500x20960 PNG image can be output.
![Preview](https://raw.githubusercontent.com/hotstaff/logistic_opencv/master/logistic3-4-2560x2048-st_sam.png)

# make (ubuntu)

```bash
sudo apt install libopencv-dev 
git clone https://github.com/hotstaff/logistic_opencv.git
cd logistic_opencv
make
```
# usage

```bash
./logistic_opencv
```
Default

 - a = 3.5 to a = 4
 - skip = 8 -> density = 1/skip = 1/8
 - 26500x20960 size PNG image.
 - interation 10000

```bash
./logistic_opencv 3.5 4 8 1024 768 100000
```
This option means

- a = 3.5 to a = 4
- skip 8
- 1024x768 size PNG image
- interation 100000

# License
MIT
