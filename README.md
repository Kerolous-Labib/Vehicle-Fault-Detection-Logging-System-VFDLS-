<h1 align="center">🚗 Vehicle Fault Detection and Logging System (VFDLS)</h1>

<p>
The <b>Vehicle Fault Detection and Logging System (VFDLS)</b> is an embedded systems project
based on a <b>dual ATmega32 ECU architecture</b>. It continuously monitors critical vehicle
parameters, detects abnormal operating conditions in real time, logs faults permanently,
and reports system status clearly to the user.
</p>

<hr>

<h2>🧩 System Architecture</h2>

<ul>
  <li>
    <b>HMI ECU (ATmega32)</b>
    <ul>
      <li>4×4 Keypad and 4×16 LCD interface</li>
      <li>Menu-driven user interaction</li>
      <li>Displays live sensor data (Temperature, Distance, Window States)</li>
      <li>Retrieves and scrolls stored Diagnostic Trouble Codes (DTCs)</li>
      <li>UART communication with Control ECU</li>
    </ul>
  </li>

  <li>
    <b>Control ECU (ATmega32)</b>
    <ul>
      <li>Reads sensors (LM35 Temperature & Ultrasonic Distance)</li>
      <li>Controls actuators (DC motors for car windows)</li>
      <li>Real-time fault detection and diagnostics</li>
      <li>Permanent fault logging using external EEPROM (24C16 via I2C)</li>
      <li>Sends live data and fault codes to HMI ECU</li>
    </ul>
  </li>
</ul>

<hr>

<h2>⚠️ Fault Detection</h2>

<table>
  <tr>
    <th>Fault</th>
    <th>DTC Code</th>
    <th>Trigger Condition</th>
  </tr>
  <tr>
    <td>Accident Risk</td>
    <td><b>P001</b></td>
    <td>Distance &lt; 10 cm</td>
  </tr>
  <tr>
    <td>Engine Overheat</td>
    <td><b>P002</b></td>
    <td>Temperature &gt; 90°C</td>
  </tr>
</table>

<hr>

<h2>🛠️ Technical Highlights</h2>

<ul>
  <li>Dual ECU communication using <b>UART with ACK-based protocol</b></li>
  <li>Permanent fault storage via <b>I2C EEPROM</b></li>
  <li>Modular drivers (GPIO, UART, ADC, Timer, PWM, EEPROM, DC Motor)</li>
  <li>Interrupt-driven timers with callback functions</li>
  <li>Layered architecture inspired by real automotive ECU design</li>
</ul>

<hr>

<p>
<b>Training Partner:</b> Edges for Training<br>
<b>Domain:</b> Embedded Systems & Automotive Electronics
</p>

<p align="center">
✨ Designed to simulate real-world automotive ECU behavior using low-level embedded C and AVR microcontrollers.
</p>
