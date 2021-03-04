using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class NPCTalk : MonoBehaviour
{
    public GameObject Talk, player, Menu, TalkMenu;
    public Text TalkText;
    public AudioSource[] Voice;

    static public bool Intro = false;
    static public bool Gossip = false;

    // Start is called before the first frame update
    void Start()
    {
        Talk.SetActive(false);
        Menu.SetActive(false);
        TalkMenu.SetActive(false);
    }

    public void TalkSelect()
    {
        player.GetComponent<PlayerControl>().Wait();
        TalkMenu.SetActive(true);
        Menu.SetActive(false);
    }

    public void IntroTalk()
    {
        Intro = true;

        if (triel.Stay)
            triel.SoundCheak = true;
        if (cain.Stay)
            cain.SoundCheak = true;

        TalkMenu.SetActive(false);
        Talk.SetActive(true);
        TalkText.GetComponent<RectTransform>().localPosition = new Vector3(-1.26f, -240.4f, 0);
    }

    public void GossipTalk()
    {
        Gossip = true;

        if (triel.Stay)
            triel.SoundCheak = true;
        if (cain.Stay)
            cain.SoundCheak = true;

        TalkMenu.SetActive(false);
        Talk.SetActive(true);
        TalkText.GetComponent<RectTransform>().localPosition = new Vector3(-1.26f, -240.4f, 0);
    }

    public void QuitSelect()
    {
        player.GetComponent<PlayerControl>().Wait();
        if (Menu.active)
            Menu.SetActive(false);
        if (TalkMenu.active)
        {
            TalkMenu.SetActive(false);
            Menu.SetActive(true);
        }
        player.GetComponent<PlayerControl>().Play();
    }

    // Update is called once per frame
    void Update()
    {     
        if (Input.GetMouseButtonDown(0))
        {
            if (Intro || Gossip)
            {
                Talk.SetActive(false);
                TalkMenu.SetActive(true);

                Intro = false;
                Gossip = false;
            }
        }
    }
}
