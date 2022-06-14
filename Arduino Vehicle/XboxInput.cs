using System.Collections;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(SerialController))]
public class XboxInput : MonoBehaviour
{
    #region declarations

    const int _maxIntSignal = 255;

    [SerializeField] [Range(1, 30)] int _signalRate = 10;
    [SerializeField] [Min(1)] int _stciksUiOffset = 10;
    [Space]
    [SerializeField] Text _leftX;
    [SerializeField] Text _leftY, _rightX, _rightY, _debug;
    [SerializeField] RectTransform _leftStick, _rightStick;

    SerialController controller;
    string signal = ""; // 4 int signals in region -255 - 255

    #endregion

    private void Awake()
    {
        controller = GetComponent<SerialController>();
    }
    void OnMessageArrived(string msg)
    {
        Debug.Log("Arrived: " + msg);
    }
    void OnConnectionEvent(bool success)
    {
        Debug.Log(success ? "Device connected" : "Device disconnected");
        if (success) StartCoroutine(update());
        else StopAllCoroutines();
    }

    IEnumerator update()
    {
        while(true)
        {
            // TODO почему четвертый канал плохо передается?
            float[] axes = { Input.GetAxis("Horizontal"), Input.GetAxis("Vertical"), Input.GetAxis("Stick Y"), Input.GetAxis("Stick X") };

            byte[] signals = new byte[4];
            signal = "";
            for (int i = 0; i < axes.Length; i++)
            {
                signals[i] = (byte)((axes[i] + 1) / 2 * _maxIntSignal); // 0 - 255
                signal += signals[i] + (i == axes.Length - 1 ? ";" : ",");
            }
            controller.SendSerialMessage(signal);

            #region visual

            _leftStick.anchoredPosition = new Vector2(axes[0] * _stciksUiOffset, axes[1] * _stciksUiOffset);
            _rightStick.anchoredPosition = new Vector2(axes[2] * _stciksUiOffset, axes[3] * _stciksUiOffset);

            _leftX.text = signals[0].ToString();
            _leftY.text = signals[1].ToString();
            _rightX.text = signals[2].ToString();
            _rightY.text = signals[3].ToString();
            _debug.text = signal;

            #endregion

            yield return new WaitForSeconds(1 / (float)_signalRate);
        }
    }

    private void OnDestroy()
    {
        StopAllCoroutines();
    }
}