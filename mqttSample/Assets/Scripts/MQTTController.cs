using System;
using System.Threading;
using System.Threading.Tasks;
using MQTTnet;
using MQTTnet.Client;
using UniRx;
using UnityEngine;

public class MQTTController : MonoBehaviour
{
    IMqttClient mqttClient;
    public Subject<string> OnMessageReceived{get; private set;} = new Subject<string>();
    
    // Start is called before the first frame update
    async void Start()
    {
        var context = SynchronizationContext.Current;

        var factory = new MqttFactory();
        mqttClient = factory.CreateMqttClient();

        var options = new MqttClientOptionsBuilder()
            .WithTcpServer("192.168.100.50", 1883)
            // .WithClientId(Guid.NewGuid().ToString())
            // .WithTls()
            .Build();

        mqttClient.Connected += async (s, e) =>
        {
            Debug.Log("Connected to MQTT broker.");
            await mqttClient.SubscribeAsync(
                new TopicFilterBuilder().WithTopic("m5stack/control/#").Build());
            Debug.Log("Subscribed topic");
        };

        mqttClient.Disconnected += async (s, e) =>
        {
            if (e.Exception == null)
            {
                Debug.Log("Shutdown connection with server.");
                return;
            }

            Debug.Log("Shutdown connection with server. retry connect afeter 5 seconds");
            await Task.Delay(TimeSpan.FromSeconds(5));

            try
            {
                await mqttClient.ConnectAsync(options);
            }
            catch
            {
                Debug.Log("faied to retry connect.");
            }
        };

        mqttClient.ApplicationMessageReceived += (s, e) =>
        {
            var message = System.Text.Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
            Debug.Log($"message received: {message}");
            context.Post( _ =>
            {
                OnMessageReceived.OnNext(message);
            }, null);
        };

        await mqttClient.ConnectAsync(options);
    }

    async void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            var message = new MqttApplicationMessageBuilder()
                .WithTopic("m5stack/control/1")
                .WithPayload("Hello World")
                .WithExactlyOnceQoS()
                .Build();

            await mqttClient.PublishAsync(message);           
        }
    }

    async private void OnApplicationQuit()
    {
        await mqttClient.DisconnectAsync();
    }
}
