const char webserverPostTest2B5[] =
R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitoring Polusi Suara</title>
    <!-- CSS -->
    <style>
        h1,p{
            text-align: center;
            font-family:system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
        }   
    </style>
</head>
<body>
    <h1>Monitoring Polusi Suara</h1>
    <p>
        Data didapatkan dari platform ThingSpeak yang terhubung dengan device IoT</br>
        <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2090722/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe></br>
        © Kelompok B5 </br>
        IoT B 2023 </br>
    </p>
    <!-- JS -->
<script>
    // TODO: Set Field in ThingSpeak
const API_KEY = "https://api.thingspeak.com/update?api_key=P6MZ3YVRLBOGTU0P&field1=0";
const CHANNEL_ID = "2090722";

// DO NOT TOUCH THIS CODE
// Set field data to Thingspeak
const setData = async (args) => {
    // Concat fields into strings of request queries
    let query = "";
    for (const field in args) {
        query += `&${field}=${args[field]}`;
    }
    console.log(query);

    const BASE_URL = `https://api.thingspeak.com/update?api_key=${API_KEY}${query}`;
    const response = await fetch(BASE_URL);
    const data = await response.json();
    return data;
}

// DO NOT TOUCH THIS CODE
// Monitor Data ThingSpeak
const getData = async () => {
    const BASE_URL = `https://api.thingspeak.com/channels/${CHANNEL_ID}/feeds.json?results=1`;
    const response = await fetch(BASE_URL);
    const data = await response.json();
    return data;
}

// TODO: Call setData() and getData for your needs.
// To set data, set ALL fields. Otherwise there will be field with NULL value.
await setData({
    field1: 0,
});

// Get Data ThingSpeak
await getData();
</script>
</body>
</html>
)=====";
