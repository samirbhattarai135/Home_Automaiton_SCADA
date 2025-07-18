# Smart Home SCADA Dashboard

![Smart Home Dashboard Screenshot](https://github.com/samirbhattarai135/Home_Automaiton_SCADA/blob/main/Dashboard_image.png) 

A comprehensive SCADA dashboard for smart home automation, built using the Ignition Perspective module. This project provides real-time monitoring, device control, and historical data analysis for a typical home environment, with data sourced from ESP32 microcontrollers via MQTT and Node-RED.

---

## ✨ Features

- **Real-Time Room Monitoring:** At-a-glance status for multiple rooms, including:
  - Living Room
  - Bedroom
  - Kitchen
- **Device Control:** Toggle switches for controlling devices like lights, fans, and stoves directly from the dashboard.
- **Centralized Alarm Panel:** A top-level view for critical system alerts, including Fire, Humidity, and Electrical alarms.
- **Historical Trend Chart:** An interactive time-series chart to visualize historical sensor data (e.g., Temperature, Humidity) over various time ranges.
- **Energy Management:** A dedicated panel to monitor:
  - **Real-time** power consumption.
  - **Historical** energy usage with a daily bar graph.
- **Interactive Floor Plan:** A visual, map-based view of the home for intuitive device location and status checks.
- **Mobile Responsiveness:** Implement a Breakpoint Container to create a tailored layout for mobile devices.

---

## 🛠️ Technical Stack

- **Platform:** [Ignition by Inductive Automation](https://inductiveautomation.com/) (Version 8.1.x)
- **SCADA Module:** **Ignition Perspective**
- **Hardware:** **ESP32, Flame Sensor, Temp/Humidity Sensor, Leds**  for sensor data collection, forwarding, and actuation.
- **Communication:** **MQTT Module** for lightweight broker service and messaging.
- **Middleware:** **Node-RED** for data processing and routing.
- **Database:** **SQLite** for Tag History logging.

---

## 🏗️ System Architecture

The data flows from the physical sensors to the dashboard following this architecture:

1.  **ESP32 & Sensors:** An ESP32 microcontroller reads data from various sensors (Flame, temperature, humidity etc.).
2.  **MQTT Broker:** The ESP32 publishes this data to specific topics on an MQTT broker, and also subcribes to Ignition's MQTT Engine module via Node-Red.
3.  **Node-RED:** A Node-RED instance subscribes to the Ignition Topics, processes the data (e.g., formatting JSON), and then forwards it to MQTT.
4.  **Ignition MQTT Engine:** Ignition's MQTT Engine module publishes to the topics from Node-RED, creating and updating Ignition tags automatically.
5.  **Perspective Dashboard:** The dashboard components bind to these Ignition tags to display the live data.


## Node-Red Architecture
![Node-Red Diagram](https://github.com/samirbhattarai135/Home_Automaiton_SCADA/blob/main/Node-Red_Configs/Connection%20Image.png?raw=true) 

---

## 🚀 Setup and Installation

To get this project running in your own Ignition gateway, follow these steps:

### 1. Prerequisites

- A running Ignition Gateway (Version 8.1 or newer) with the **MQTT Engine** module installed.
- Ignition Designer installed.
- An **ESP32** board with sensors and the appropriate firmware to publish MQTT messages.
- A running **MQTT Broker** (e.g., Mosquitto).
- A running **Node-RED** instance with the necessary nodes (`node-red-contrib-mqtt-broker`).
- The `sqlite-jdbc.jar` file for the database connection (if not already present).

### 2. Import the Project

1.  Download the project backup (`.zip` file) from this repository.
2.  Open your Ignition Gateway's web interface.
3.  Go to **Config** -> **System** -> **Projects**.
4.  Click **Import Project** and upload the `.zip` file.

### 3. Configure the Database

1.  In the Gateway web interface, go to **Config** -> **Databases** -> **Connections**.
2.  Create a new database connection.
3.  Select the **SQLite** driver.
4.  Give the connection a name (e.g., `SQLite_History`).
5.  In the `Connect URL` field, specify the path to your `.db` file:
    ```
    jdbc:sqlite:C:\ProgramData\Inductive Automation\Ignition\data\your_database_name.db
    ```
6.  Save the connection.

### 4. Configure the Tag History Provider

1.  In the Gateway web interface, go to **Config** -> **Tags** -> **History**.
2.  Create a new Tag History Provider.
3.  Select the database connection you created in the previous step.
4.  Save the provider. Ensure your tags are configured to use this provider for history logging.

### 5. Launch the Project

- Open the project in **Designer** to review the views and bindings.
- Launch a **Perspective Session** to view the live dashboard in your browser.

---

## 📂 Project Structure

- **`/views/MainDashboard`**: The primary view containing all the different panels and containers.
- **`/views/templates/RoomCard`**: A reusable template for displaying room-specific information.
- **`/tags/`**: User-Defined Types (UDTs) are used to structure tags for each room (e.g., `RoomUDT` containing Temperature, Humidity, and device tags).
- **`/named-queries/`**: Contains the SQL queries used to aggregate data for the Energy Management bar chart.
---

## 🤝 Contributing

Contributions are welcome! If you have ideas for improvements or find any issues, please open an issue or submit a pull request.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

---

## 📄 License

Distributed under the MIT License. See `LICENSE` for more information.
