# ESP32_DATATOCLOUD

Der Quellcode für das Programm ist oben angeheftet und komplett auskommentiert.

Das "NH3_ROOT_includieren" Programm beinhaltet die Grundfunktionen die wir in das Programm der anderen Gruppe "NH3 Sensor" eingefügt haben.

Das "NH3_Measurement_Cloud" Programm ist das entgültige Programm, welches auf dem Esp32 des Sensoraufbaut läuft.

Die Grundfunktion der Teilaufgabe des in die Cloud schreiben ist, dass eine WiFi verbindung aufgebaut wird, z.B. über das WLAN zuhause. Wir haben zum Testen ein Handy HotSpot und das verschiedene WiFi netzwerke an der Hochschule getestet.
Leider haben wir keine möglichkeit gefunden uns mit dem KA-WLAN oder dem HSKA 8021x Netzwerk zu verbinden.
Bei dem HSKA 8021x Netzwerk war das Problem, dass ein Benutzername und Passwort benötigt wird.
Bei dem KA-WLAN Netzwerk war das Problem, dass der ESP sich grundsätzlich mit dem WLAN verbinden kann, doch muss man wenn die Verbindung steht z. B. am Computer auf der Hompage des WLANs auf den Start Butten klicken was am ESP leider nicht funktioniert.

Ist die verbindung erfolgreich aufgebaut ruft das Programm in einer Schleife die Funktion auf, mit welcher Daten an die Google Cloud gesendet werden können.

Das ganze Passiert mit einer HTTP GET anfrage an den Google Script Server, dort haben wir nähnlich ein Programm hinterlegt welches über eine URL Parameter empfängt und in einem Google Sheet speichert.
Damit das Programm weiß wo es das Google Script erreicht muss der Idenditäts Schlüssels des Google Scripts in die Globale Variable Key gespeichert werden.

Wie das Google Script auszusehen hat findet man hier: http://esp32-server.de/google-sheets/

Damit das Programm läuft, muss das eigene WLAN in die entsprechenden Zeilen(mit kommentar versehen) eingetragen werden.


