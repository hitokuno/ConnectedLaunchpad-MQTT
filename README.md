# ConnectedLaunchpad-MQTT
=====================

Hello world for IBM Bluemix IoT foundation on TI Connected Launchpad.  
This small sketch sends the temperature and status of the user switches on board.

Ingredient
--------
* [TI Connected LaunchPad(EK-TM4C1294XL)](http://www.ti.com/ww/en/launchpad/launchpads-connected-ek-tm4c1294xl.html)
* [Energia 0101E0016](http://energia.nu/)
* LAN to connect

Install
--------
1. Install Energia on PC or Mac.
2. Copy all lines of ConnectedLaunchpad-MQTT.ino  
3. Paste to new file in Energia.  
4. Change deviceId at line 14 from Change-to-your-mac-address to your mac address of Connected Launchpad.  
5. Select LaunchPad(Tiva C) /w tm4c129(120MHz) from Tools/Board.  
6. Connect LaunchPad to USB port.  
7. Select port from Tools/Port.  
8. Click upload button.  
9. Open [http://quickstart.internetofthings.ibmcloud.com/#/device/](http://quickstart.internetofthings.ibmcloud.com/#/device/)  
10. Enter your mac address to see temperature and status or user swiches(PJ1, PJ2).

Known Issue
--------
* If first connection to Bluemix failed, keep failing.  Please reset the device by pressing reset button on the board.
* Data to send shuld be less than 72 charactors.  This seems to be bug in MQTT library with Energia 0101E0016.  Future version may not have this issue.

Advanced Settings
--------
* You may change to see Digital Read from PE_0 etc., instaed of user switches.  Please remember the limit of the charactors to send.

Auther
--------
TOKUNO, Hirokazu
