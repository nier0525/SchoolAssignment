using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Option : MonoBehaviour
{
    GameObject player;
    PlayerControl player_c;
    PlayerMotion player_m;

    public GameObject option , gm;
    public AudioSource Sel;

    public GameObject focus, focus2;

    public GameObject seoption;
    public GameObject soundoption;
    public GameObject videooption;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.Find("Sorceress");
        player_c = player.GetComponent<PlayerControl>();
    }

    public void MouseEnter()
    {
        focus.SetActive(true);
        focus2.SetActive(true);
    }

    public void MouseExit()
    {
        focus.SetActive(false);
        focus2.SetActive(false);
    }

    //---------------------------------------------------------------

    public void SeOption()
    {
        Sel.Play();
        option.SetActive(false);
        seoption.SetActive(true);
    }

    public void ReturnGameOption()
    {
        Sel.Play();
        player_c.wait = true;
        focus.SetActive(false);
        focus2.SetActive(false);
        option.SetActive(false);
        //Time.timeScale = 1f;
        gm.GetComponent<GameManager>().stateinit();
    }

    public void ExitGameOption()
    {
        Sel.Play();
        //Time.timeScale = 1f;
        SceneManager.LoadScene("Title");
    }

    //----------------------------------------------------------------

    public void SoundOption()
    {
        Sel.Play();
        seoption.SetActive(false);
        soundoption.SetActive(true);
    }

    public void VidioOption()
    {
        Sel.Play();
        seoption.SetActive(false);
        videooption.SetActive(true);
    }

    public void PreviousMenu()
    {
        Sel.Play();
        seoption.SetActive(false);
        option.SetActive(true);
    }

    public void PreviousSeMenu()
    {
        Sel.Play();
        soundoption.SetActive(false);
        seoption.SetActive(true);
    }

    public void PreviousViMenu()
    {
        Sel.Play();
        videooption.SetActive(false);
        seoption.SetActive(true);
    }

    //------------------------------------------------------------------

    //------------------------------------------------------------------

    // Update is called once per frame
    void Update()
    {
        
    }
}
