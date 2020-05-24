using System;
using TMPro;
using UniRx;
using UnityEngine;

public class MessageController : MonoBehaviour
{
    [SerializeField] private MQTTController mQTTController;
    [SerializeField] private GameObject tmObj;

    void Start()
    {
        mQTTController.OnMessageReceived.Subscribe (message => 
        {
            // var generatedObject = Instantiate(
            //     tmObj,
            //     tmObj.transform.position,
            //     tmObj.transform.rotation
            // );
            // if(tmObj is null)
            // {
                Debug.Log("received");
                Debug.Log(tmObj);
            // }
            // else{
                // tmObj.GetComponent<TextMeshPro>().text = message;
            // }        

            // var generatedObjectRb = generatedObject.GetComponent<Rigidbody>();
            // generatedObjectRb.useGravity = false;
            // generatedObjectRb.AddForce(0f, 3f, 0f, ForceMode.Impulse);
            // GameObject.Destroy(generatedObject, 3f);
        });
    }
}