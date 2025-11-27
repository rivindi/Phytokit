<?php

$servername = "123.45.67.89"; // IP address of external DB server

$dbname = "phytokit";
$username = "kaushini";
$password = "Kaushi@1234";

// API key for validating requests from NodeMCU/ESP devices
$api_key_value = "tPmAT5Ab3j7F9";

// Variables to store POST data
$api_key = $sensor = $location = $value1 = $value2 = $value3 = "";

// Validate if data is posted via HTTP POST method
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);

    // Check if the API key matches
    if ($api_key == $api_key_value) {

        // Collect and sanitize POST values
        $sensor   = test_input($_POST["sensor"]);
        $location = test_input($_POST["location"]);
        $value1   = test_input($_POST["value1"]); // e.g., humidity
        $value2   = test_input($_POST["value2"]); // e.g., temperature
        $value3   = test_input($_POST["value3"]); // e.g., soil moisture

        // Create DB connection
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Check DB connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // SQL INSERT statement
        $sql = "INSERT INTO SensorData (sensor, location, value1, value2, value3) 
                VALUES ('$sensor', '$location', '$value1', '$value2', '$value3')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

        // Close DB connection
        $conn->close();
    } else {
        echo "Wrong API Key provided.";
    }
} else {
    echo "No data posted with HTTP POST.";
}

// Sanitize function
function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
