<a id="readme-top"></a>

<img src="./readme/loop.gif" width="100%">

<br />
<div align="center">

  # [miniDAISY](https://github.com/lupoqul/miniDAISY)

  <p align="center">
    a Tiny esp32-based BVG-style train information display
    <br />
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#hardware">Hardware</a></li>
    <li><a href="#software">Software</a></li>
    <li><a href="#future-goals">Future Goals</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

Originally, this was a gift for a friend. It closely resembles the train information displays (called DAISY) used by the BVG in Berlin. They show a line number, destination, arrival time and the length of the train. I tried to turn them into a decorative piece. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Hardware

Its based on an ESP32-board. I've chosen this one, because there is a lipo charger included.

* AZDelivery LOLIN32 Lite Board V1.0
* Waveshare 2.42inch OLED Display Module 128×64
* generic 820mAh li-po battery

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Software

As an IDE I'm using [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO IDE](https://platformio.org/platformio-ide). The platformio.ini configuration file is included. It contains a build configuration and the following libraries:

* [bblanchon/ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [olikraus/U8g2](https://github.com/olikraus/u8g2)
* [rlogiacco/CircularBuffer](https://github.com/rlogiacco/CircularBuffer)

As of now, the train live data is supplied by [v6.vbb.transport.rest](https://v6.vbb.transport.rest/). Unfortunately there is no way to get information about the length or type of train. But it does return the line number, planned and actual time, destination and some more. The esp requests new data every 3min and stores it for the next 12 trains.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Future Goals

As of now, this is just a tiny software project without any testing. Its just a proof of work. Future Goals include:

* 3d-print case
* code testing
* demo mode without wifi

<p align="right">(<a href="#readme-top">back to top</a>)</p>