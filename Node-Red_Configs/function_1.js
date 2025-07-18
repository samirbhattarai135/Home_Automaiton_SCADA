node.warn("Raw message received");

// Simple approach: Look for "LightStatus" in the raw payload
if (msg.payload) {
  // Converting buffer to string to search for our tag name
  let payloadStr = msg.payload.toString();

  // Checking if this message contains LightStatus
  if (payloadStr.includes("LightStatus")) {
    node.warn("Found LightStatus in message");
    if (global.get("ledState")) {
      global.set("ledState", false);
      msg.payload = "ON";
    } else {
      global.set("ledState", true);
      msg.payload = "OFF";
    }

    msg.topic = "esp32/led";
    node.warn("Sending: " + msg.payload);
    return msg;
  }
}

return null;
