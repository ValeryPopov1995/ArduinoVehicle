using System.Linq;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(RawImage))]
public class WebTexture : MonoBehaviour
{
    RawImage _raw;
    WebCamTexture _texture;

    private void Start()
    {
        foreach (var c in WebCamTexture.devices)
            Debug.Log(c.name);

        if (WebCamTexture.devices.Length > 0)
            SetTexture(WebCamTexture.devices[0]);
    }

    public void _SwitchWebCam()
    {
        var devicesEnumerator = WebCamTexture.devices.Where(x => new WebCamTexture(x.name) != null); // TODO How check not virtual cam?
        var devices = devicesEnumerator.ToArray();
        if (devices.Length > 1)
        {
            int usedDeviceIndex = 0;
            for (int i = 0; i < devices.Length; i++)
                if (devices[i].name == _texture.deviceName)
                    usedDeviceIndex = i;

            int newDeviceIndex = usedDeviceIndex == (devices.Length - 1) ? 0 : usedDeviceIndex + 1;
            SetTexture(devices[newDeviceIndex]);
        }
    }

    private void SetTexture(WebCamDevice device)
    {
        if (_texture != null && _texture.isPlaying)
            _texture.Stop();

        _texture = new WebCamTexture(device.name);
        _texture.requestedFPS = 30;
        _raw = GetComponent<RawImage>();
        _raw.texture = _texture;
        _texture.Play();
    }
}
