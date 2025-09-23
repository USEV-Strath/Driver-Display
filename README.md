# Driver-Display

Code to display data on the 2 4x20 LCD displays from the car to the driver.

## Upload Instructions (Linux)

* Ensure you have [Arduino Cli](https://docs.arduino.cc/arduino-cli/) installed.
* Update local cache:
  ```
  arduino-cli core update-index
  ```
* Add the Liquid Crystal Library that we use:
  ```
  arduino-cli lib install "LiquidCrystal I2C"
  ```
* The Wire library should be included so you don't need to install it separately.
* Clone the repo and navigate to the `USEV-Driver-Display` directory.

<br>

### Compilation Only

If you only want to compile the code to check that it works, not upload it:

* Install Arduino boards: 
  ```
  arduino-cli core install arduino:avr
  ```
* Locate the FQBN of the board from the list that you wish to compile for:
  ```
  arduino-cli board listall
  ```
* We are using an Arduino Uno, so we use `arduino:avr:uno`
* Compile the code, here we are using an Arduino Uno:
  ```
  arduino-cli compile -b arduino:avr:uno display_4x20 --build-path build
  ```
* This compiles the code for an Arduino Uno, storing the build in the `build` folder.

<br>

### Uploading 
If you want to compile and upload the code to a board:
* Connect the Arduino to your computer
* List the available boards and where they are conected using:
  ```
  arduino-cli board list
  ```
  From this, note the `FQBN` and `Port`
* Using the `FQBN`, install the core for your board:
  ```
  arduino-cli core install FQBN
  ```
  where `FQBN` is your board type, eg. `arduino:avr:uno`
* Compile the code, replacing `FQBN` with your board type:
  ```
  arduino-cli compile -b FQBN display_4x20
  ```
* Finally, compile and upload your code using:
  ```
  arduino-cli upload -p Port -b FQBN display_4x20
  ```
  Where `Port` and `FQBN` are replaced with the values from earlier.
  <br>
  **Note: If you get a permission error upon upload, such as  `Error opening serial port...`, you may need to change permissions.** See the "Please Read" section of [this](https://docs.arduino.cc/software/ide-v1/tutorials/Linux/) page.
<br>

* To upload a new version, simply run the last 2 steps again 

<br>

----------

If you don't have linux or don't want to use the cli, you may use the [Arduino IDE](https://docs.arduino.cc/software/ide/) instead if you wish
