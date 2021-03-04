using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SoundManager : MonoBehaviour
{
    public Slider backVolume;
    public Slider soundVolume;
    public AudioSource[] bgm = new AudioSource[4];
    public AudioSource[] sound = new AudioSource[3];
    public AudioSource[] voice = new AudioSource[10];

    public Sprite on, off;

    float backVol;
    float soundVol;
    float voiceVol = 0.8f;

    int state = 0;

    // Start is called before the first frame update
    void Start()
    {
        backVol = PlayerPrefs.GetFloat("backvol", 0.8f);
        soundVol = PlayerPrefs.GetFloat("soundvol", 0.8f);

        backVolume.value = backVol;
        for (int i = 0; i < bgm.Length; i++) {
            bgm[i].volume = backVolume.value;
        }

        soundVolume.value = soundVol;
        for (int j = 0; j < sound.Length; j++)
        {
            sound[j].volume = soundVolume.value;
        }
    }

    public void BGMSlider()
    {
        for (int i = 0; i < bgm.Length; i++)
        {
            bgm[i].volume = backVolume.value;
        }
        backVol = backVolume.value;
        PlayerPrefs.SetFloat("backvol", backVol);
        PlayerPrefs.Save();
    }

    public void SoundSlider()
    {
        for (int i = 0; i < sound.Length; i++)
        {
            sound[i].volume = soundVolume.value;
        }
        soundVol = soundVolume.value;
        PlayerPrefs.SetFloat("soundvol", soundVol);
        PlayerPrefs.Save();
    }

    public void VoiceOnOff()
    {
        GameObject.Find("OptionSelectSound").GetComponent<AudioSource>().Play();
        state++;

        if (state % 2 == 1)
        {
            gameObject.GetComponent<Image>().sprite = off;
            for (int i = 0; i < voice.Length; i++)
            {
                voice[i].volume = 0f;
            }
        }

        else
        {
            gameObject.GetComponent<Image>().sprite = on;
            for (int i = 0; i < voice.Length; i++)
            {
                voice[i].volume = voiceVol;
            }
            state = 0;
        }
    }

    public void SoundCheak()
    {
        sound[0].Play();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
       // BGMSlider();
    }
}
