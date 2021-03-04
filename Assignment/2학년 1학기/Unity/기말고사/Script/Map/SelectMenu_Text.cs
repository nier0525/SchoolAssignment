using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SelectMenu_Text : MonoBehaviour
{
    public AudioSource Sound;
    public static bool cheak = false;

    // Start is called before the first frame update
    void Start()
    {

    }

    public void In()
    {
        cheak = true;
    }

    public void Out()
    {
        cheak = false;
    }

    public void Enter()
    {
        Sound.Play();
        gameObject.GetComponent<Text>().color = new Color(0.4f, 0.4f, 1f);
    }

    public void Quit()
    {
        gameObject.GetComponent<Text>().color = new Color(1f, 1f, 1f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
