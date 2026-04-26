# BMI160
A small library for work with BMI160 chip
## How to use
 1. Announcment
    -
    In library exist 3 type of object:
     - BMI160 - main class, which define behavior of chip
        + > BMI160 {BMI_Name};
     - Offset - structure, that save offset of calibration.
        + > Offset {Off_Name};
     - SensorData - structure, that save final data. (Use majority in loop)
        + > SensorData {Data_Name} = {BMI_Name}.readCalibrated(off);

    This Distinction between objects help to modify library in future.
    
    ---
 2. Setup:
    -
    - Init SCA/SDA pins:
        + > Wire.begin({SDA_pin}, {SCL_pin});
    - Init BMI:
        + > {BMI_Name}.init({Accel_g}, {Gyro_freq}); // in default 4g, 500 Degrees per sec.
    - Test physical connection(Optional):
        + > {BMI_name}.connectionTest(); // if print 209 - connection fine.
    - Fill offset with data:
        + > {Off_Name} = {BMI_Name}.callibrate();
    ---
 3. Loop:
    -
    - SensorData - read datas
        + > SensorData {Data_Name} = {BMI_Name}.readCalibrated(off);
    - Values use:
        + > {Data_Name}.(ax/ay/az(Accelerometr))/(gx,gy,gz(Gyroscope)); //Example: d.gx mean data of coordinates gyro to the x-axis.

## Q&A
 > I have: "error: 'BMI160' does not name a type BMI160 {Name};", what does it means?
 ---
 If you have another BMI160 Library, try to remove them, and compilate sketch again.
 > From where you get register to chip?
 ---
 Datasheet: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi160-ds000.pdf
 
 2.11 Register map.

 ## Library by: IamWerty