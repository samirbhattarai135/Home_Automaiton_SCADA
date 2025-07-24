// Debug: Log the raw message
node.warn("Raw message received: " + msg.topic);

// Simple approach: Look for device status tags in the raw payload
if (msg.payload) {
  // Convert buffer to string to search for our tag names
  let payloadStr = msg.payload.toString();

  // Check if this message contains LightStatus
  if (payloadStr.includes("LightStatus")) {
    node.warn("Found LightStatus in message");

    let ledMsg = { topic: "esp32/led", payload: "" };

    if (global.get("ledState")) {
      global.set("ledState", false);
      ledMsg.payload = "OFF";
    } else {
      global.set("ledState", true);
      ledMsg.payload = "ON";
    }

    node.warn("Sending LED: " + ledMsg.payload);
    return [ledMsg, null]; // Output 1 for LED
  }

  // Check if this message contains FanStatus
  if (payloadStr.includes("FanStatus")) {
    node.warn("Found FanStatus in message");

    let fanMsg = { topic: "esp32/fan", payload: "" };

    if (global.get("fanState")) {
      global.set("fanState", false);
      fanMsg.payload = "OFF";
    } else {
      global.set("fanState", true);
      fanMsg.payload = "ON";
    }

    node.warn("Sending Fan: " + fanMsg.payload);
    return [fanMsg, null]; // Output 2 for Fan
  }
}

return [null, null];
