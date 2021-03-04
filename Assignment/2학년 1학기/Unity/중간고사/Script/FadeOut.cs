using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeOut : MonoBehaviour
{
    public float animTime = 2f;
    private Image fadeImage;

    private float start = 0f;
    private float end = 1f;
    private float time = 0f;

    private bool isPlaying = false;

    // Start is called before the first frame update
    void Awake()
    {
        fadeImage = GetComponent<Image>();
        Application.targetFrameRate = 60;
    }

    public void StartFadeAnim()
    {
        if (isPlaying == true)
            return;
        StartCoroutine("PlayFadeOut");
    }

    IEnumerator PlayFadeOut()
    {
        isPlaying = true;

        Color color = fadeImage.color;
        time = 0f;
        color.a = Mathf.Lerp(start, end, time);

        while (color.a < 1f)
        {
            time += Time.deltaTime / animTime;
            color.a = Mathf.Lerp(start, end, time);
            fadeImage.color = color;

            yield return null;
        }
        isPlaying = false;
    }

    public float GetColor()
    {
        return time;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
