using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraSize : MonoBehaviour
{
    private void Awake()
    {
        Camera camera = GetComponent<Camera>();
        Rect rect = camera.rect;

        float height = ((float)Screen.width / Screen.height) / ((float)16 / 9);
        float width = 1f / height;

        if (height < 1f)
        {
            rect.height = height;
            rect.y = (1f - height) / 2f;
        }
        else
        {
            rect.width = width;
            rect.x = (1f - width) / 2;
        }

        camera.rect = rect;
    }
}
