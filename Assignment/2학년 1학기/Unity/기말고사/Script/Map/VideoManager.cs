using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class VideoManager : MonoBehaviour
{
    public Slider LightVal;
    public Image Light;
    Color c;

    GameObject[] shadowObj;
    public Sprite on, off;

    float LightValue = 0.15f;
    int state = 0;

    // Start is called before the first frame update
    void Start()
    {
        LightValue = PlayerPrefs.GetFloat("lightvalue", 0.15f);
        LightVal.value = LightValue;
        c = new Color(0, 0, 0, LightVal.value);
        Light.color = c;

        shadowObj = GameObject.FindGameObjectsWithTag("Shadow");
    }

    public void LightSlider()
    {
        LightValue = LightVal.value;
        c = new Color(0, 0, 0, LightValue);
        Light.color = c;
        PlayerPrefs.SetFloat("lightvalue", LightValue);
        PlayerPrefs.Save();
    }

    public void ShadowOnOff()
    {
        GameObject.Find("OptionSelectSound").GetComponent<AudioSource>().Play();
        state++;

        if (state % 2 == 1)
        {
            gameObject.GetComponent<Image>().sprite = off;
            for (int i = 0; i < shadowObj.Length; i++)
            {
                shadowObj[i].SetActive(false);
            }
        }

        else
        {
            gameObject.GetComponent<Image>().sprite = on;
            for (int i = 0; i < shadowObj.Length; i++)
            {
                shadowObj[i].SetActive(true);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
