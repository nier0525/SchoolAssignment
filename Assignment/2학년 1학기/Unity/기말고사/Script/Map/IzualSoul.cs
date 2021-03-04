using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IzualSoul : MonoBehaviour
{
    public GameObject Balloon;
    public GameObject FadeIn_Sound, FadeOut_Sound;
    public static Animator ani;
    SpriteRenderer Izual_spr;
    public GameObject name;

    Color c, c_temp;

    public static bool clear = true;
    public static bool Enter = false;
    public static bool SoulExit = false;

    // Start is called before the first frame update
    void Start()
    {
        ani = gameObject.GetComponent<Animator>();
        Izual_spr = gameObject.GetComponent<SpriteRenderer>();

        FadeOut_Sound.GetComponent<AudioSource>().Play();
        ani.SetBool("Stay", true);
        ani.SetBool("FadeIn", false);

        c_temp = Izual_spr.color;
        name.SetActive(false);
        Balloon.SetActive(false);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
            Enter = true;
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            Enter = false;
        }
    }

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        Izual_spr.color = c;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        Izual_spr.color = c;
    }

    IEnumerator Destroy()
    {
        ani.SetBool("Stay", false);
        ani.SetBool("FadeIn", true);
        FadeIn_Sound.GetComponent<AudioSource>().Play();
        yield return new WaitForSeconds(1.2f);
        SoulExit = true;
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (Quest.Q == 2)
        {
            Balloon.SetActive(true);
        }
        else
        {
            Balloon.SetActive(false);
        }

        if (Quest.Q == 3 && !clear)
        {
            StartCoroutine(Destroy());
            clear = true;
        }
    }
}
