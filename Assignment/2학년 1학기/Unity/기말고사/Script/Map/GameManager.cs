using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public GameObject Diablo, DiabloSound;

    GameObject player, cam;
    PlayerControl player_c;
    PlayerMotion player_m;

    public GameObject Trap01, Trap02, Trap03;

    bool DemonOn, GhostOn, LoadOn;
    bool TrapOn;

    public GameObject Option , SeOption, SoundOption, VideoOption, GameOver;
    public GameObject[] Focus_d = new GameObject[6];
    public AudioSource OnOff;

    public GameObject Inven, skill, status, skill_left, skill_right, Quest, NPC_menu01, NPC_menu02, Talk;

    int state = 0;
    float x, y;

    float init_diablo, init_Izual, init_Hepasto;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.Find("Sorceress");
        player_c = player.GetComponent<PlayerControl>();
        player_m = player.GetComponent<PlayerMotion>();
        cam = GameObject.Find("Main Camera");
        
        TrapOn = false;

        init_diablo = 10000;
        init_Izual = 2000;
        init_Hepasto = 2000;
    }

    private void Awake()
    {

    }

    public void stateinit()
    {
        state = 0;
    }

    IEnumerator DiabloSummon()
    {
        int index = 0;

        while (index < 5)
        {
            cam.transform.position -= new Vector3(0.08f, 0, 0);
            yield return new WaitForSeconds(0.1f);
            cam.transform.position += new Vector3(0.08f, 0, 0);
            yield return new WaitForSeconds(0.1f);
            cam.transform.position += new Vector3(0.08f, 0, 0);
            yield return new WaitForSeconds(0.1f);
            cam.transform.position -= new Vector3(0.08f, 0, 0);

            index++;
        }
    }

    // Update is called once per frame
    void Update()
    {
        DemonOn = Trap01.GetComponent<DemonTrap>().DemonTrapOn;
        GhostOn = Trap02.GetComponent<GhostTrap>().GhostTrapOn;
        LoadOn = Trap03.GetComponent<LoadTrap>().LoadTrapOn;

        if (DemonOn && GhostOn && LoadOn /*|| Input.GetKey(KeyCode.B)*/)
        {
            Diablo.SetActive(true);       
            if (!TrapOn)
            {
                StartCoroutine(DiabloSummon());
                DiabloSound.GetComponent<AudioSource>().Play();
                TrapOn = true;
            }
        }

        if (player_c.die)
        {
            GameOver.SetActive(true);
        }

        if (Input.GetKeyDown(KeyCode.Q))
        {
            GameObject.Find("Vol").GetComponent<AudioSource>().Play();

            if (!Quest.active)
            {
                if (status.active)
                {
                    status.SetActive(false);
                }
                Quest.SetActive(true);
                cam.GetComponent<CameraSize>().Button01();
                //cam.transform.position -= new Vector3(2, 0, 0);
            }
            else
            {
                Quest.SetActive(false);
                cam.GetComponent<CameraSize>().Button01();
                player_c.wait = true;
                //cam.transform.position += new Vector3(2, 0, 0);
            }
        }

        if (Input.GetKeyDown(KeyCode.I))
        {
            GameObject.Find("Vol").GetComponent<AudioSource>().Play();

            if (!Inven.active)
            {
                if (skill.active)
                {
                    skill.SetActive(false);
                }
                PlayerControl.DontMove = true;
                Inven.SetActive(true);
                cam.GetComponent<CameraSize>().Button02();
                //cam.transform.position += new Vector3(2, 0, 0);
            }
            else
            {
                PlayerControl.DontMove = false;
                Inven.SetActive(false);
                cam.GetComponent<CameraSize>().Button02();
                player_c.wait = true;
                //cam.transform.position -= new Vector3(2, 0, 0);
            }
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (player_c.die)
            {
                SceneManager.LoadScene("Title");
            }
            else
            {
                GameObject.Find("Vol").GetComponent<AudioSource>().Play();

                if (skill.active)
                {
                    skill.SetActive(false);
                    player_c.wait = true;
                    cam.transform.position -= new Vector3(2, 0, 0);
                    cam.GetComponent<CameraSize>().status = false;
                }
                else if (skill_right.active)
                {
                    skill_right.SetActive(false);
                    player_c.wait = true;
                }
                else if (skill_left.active)
                {
                    skill_left.SetActive(false);
                    player_c.wait = true;
                }
                else if (status.active)
                {
                    status.SetActive(false);
                    player_c.wait = true;
                    cam.transform.position += new Vector3(2, 0, 0);
                    cam.GetComponent<CameraSize>().skill = false;
                }
                else if (Inven.active)
                {
                    Inven.SetActive(false);
                    PlayerControl.DontMove = false;
                    player_c.wait = true;
                    cam.transform.position -= new Vector3(2, 0, 0);
                    cam.GetComponent<CameraSize>().status = false;
                }
                else if (Quest.active)
                {
                    Quest.SetActive(false);
                    player_c.wait = true;
                    cam.transform.position += new Vector3(2, 0, 0);
                    cam.GetComponent<CameraSize>().skill = false;
                }

                else if (NPC_menu01.active)
                {
                    NPC_menu01.SetActive(false);
                    player_c.Play();
                }

                else if (NPC_menu02.active)
                {
                    NPC_menu02.SetActive(false);
                    player_c.Play();
                }

                else if (Talk.active)
                {
                    Talk.SetActive(false);
                    if (NPCTalk.Intro || NPCTalk.Gossip)
                    {
                        NPC_menu02.SetActive(true);
                        NPCTalk.Intro = false;
                        NPCTalk.Gossip = false;
                    }
                }

                else if (!skill.active && !Inven.active && !skill_left.active && !skill_right.active && !status.active && !Quest.active && !NPC_menu01.active && !NPC_menu02.active && !Talk.active && !player_c.die)
                {

                    OnOff.Play();
                    state++;

                    if (state % 2 == 1)
                    {
                        player_m.isRun = false;
                        player_m.isWalk = false;

                        player_m.animator.SetBool("Walk", false);
                        player_m.animator.SetBool("Run", false);

                        //Time.timeScale = 0f;
                        Option.SetActive(true);

                        if (Option.activeSelf)
                        {
                            player_c.wait = false;
                        }
                    }

                    else
                    {
                        player_c.wait = true;
                        for (int i = 0; i < Focus_d.Length; i++)
                        {
                            Focus_d[i].SetActive(false);
                        }
                        Option.SetActive(false);
                        SeOption.SetActive(false);
                        SoundOption.SetActive(false);
                        VideoOption.SetActive(false);
                        //Time.timeScale = 1f;
                        state = 0;
                    }

                }
            }
        }
    }
}
