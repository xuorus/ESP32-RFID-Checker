<?php
$servername = "localhost";
$username = "kurumi";
$password = "tokisaki";
$dbname = "rfid_group8";

$conn = new mysqli($servername, 
$username, $password, 
$dbname);

if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

if (isset($_POST['rfid_id'])) {
  $rfid_id = $_POST['rfid_id'];

  $sql = "SELECT status FROM rfid_db WHERE rfid_id='$rfid_id'";
  $result = $conn->query($sql);

  if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $current_status = $row['status'];

    $new_status = ($current_status == 1) ? 0 : 1;

    $update_sql = "UPDATE rfid_db SET status='$new_status' WHERE rfid_id='$rfid_id'";
    if ($conn->query($update_sql) === TRUE) {
      echo "Status: " . $new_status;
    } else {
      echo "Error updating record: " . $conn->error;
    }
  } else {
    echo "RFID NOT FOUND";
  }
} else {
  echo "No RFID ID received";
}

$conn->close();
?>
