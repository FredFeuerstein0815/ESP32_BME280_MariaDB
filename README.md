# ESP32_BME280_MariaDB_Deep_Sleep
This Script reads Temperature, Pressure and Humidity from BME280 and write the values via Wifi to MariaDB or MySQL Database.
Then the ESP32 is going to deep sleep for 30 Minutes.
You need Khoi Hoang's Library https://github.com/khoih-prog/MySQL_MariaDB_Generic and
Adafruit BME280 Library
Database and Table should be created before uploading the Code and the user should be database specific only be allowed to INSERT and SELECT.
The Server has to allow connections from the IP of the ESP32 (bind-address = 0.0.0.0)

Dieses Script liest Temperatur, Luftdruck und Luftfeuchtigkeit vom BME280 Sensor und schreibt die Werte per WLAN in eine MariaDB oder MySQL-Datenbank.
Anschliessend geht der ESP32 für 30 Minuten in den Tiefschlaf.
Du benötigst die Bibliothek von Khoi Hoang https://github.com/khoih-prog/MySQL_MariaDB_Generic und
die Adafruit BME280-Bibliothek aus der Arduino IDE.
Die Datenbank und die Tabelle mit 4 Spalten muss vor dem hochladen erstellt werden und der SQL-Benutzer sollte datenbankspezifisch nur INSERT und SELECT dürfen.
Der Server muss es erlaubt sein, Anfragen der IP des ESP32 entgegenzunehmen (bind-address = 0.0.0.0)
