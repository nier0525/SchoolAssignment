using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;

public class Player : MonoBehaviour
{
    public GameObject Fire, Skill;
    public Transform FirePos;

    public AudioSource[] SFX;

    public float firerate;
    float nextfire;

    public float JumpPower;
    Animator ani;

    static public int life;
    static public float Mana;

    bool latecheck = false;
    bool die = false;

    bool attack = false;

    Touch[] touch;

    // Start is called before the first frame update
    void Start()
    {
        ani = this.GetComponent<Animator>();
        life = 3;
        Mana = 0f;

        StartCoroutine(ManaUp());

        touch = new Touch[2];
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (!latecheck && !die && collision.gameObject.tag != "OverLine")
        {
            SFX[0].Play();
            life -= 1;

            if (life <= 0)
            {
                StartCoroutine(DieAnime());
            }
            else
            {
                ani.SetTrigger("hit");
            }

            StartCoroutine(damagelate());
        }

        if (collision.gameObject.tag == "OverLine" && !die)
        {
            SFX[0].Play();
            life -= 1;

            transform.position = new Vector3(-7.06f, 0.7f, 0);

            if (life <= 0)
            {
                StartCoroutine(DieAnime());
            }
            else
            {
                ani.SetTrigger("hit");
            }

            StartCoroutine(damagelate());
        }
    }

    IEnumerator ManaUp()
    {
        while(true)
        {
            if (Mana < 10f)
            {
                Mana += 0.1f;
            }
            yield return new WaitForSeconds(0.3f);
        }
    }

    IEnumerator DieAnime()
    {
        GetComponent<Rigidbody2D>().isKinematic = true;

        die = true;
        ani.SetTrigger("die");

        yield return new WaitForSeconds(0.7f);
        SceneManager.LoadScene("Lose");     
    }

    IEnumerator damagelate()
    {
        latecheck = true;
        GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 0.5f);
        yield return new WaitForSeconds(3f);
        GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1f);
        latecheck = false;
    }

    public void FireActive()
    {
        attack = true;
    }

    public void FireUnActive()
    {
        attack = false;
    }

    public void Skillactive()
    {
        if  (Mana >= 5f && !die)
        {
            SFX[1].Play();
            Mana -= 5f;
            Instantiate(Skill, FirePos.position, FirePos.rotation);
        }
    }

    // Update is called once per frame
    void Update()
    {
        JumpPower = PlayerPrefs.GetFloat("Jump", 5f);


        if (!GameObject.Find("GameManager").GetComponent<GameManager>().option)
        {
            // 모바일

            if (Application.platform == RuntimePlatform.Android)
            {
                if (Input.touchCount > 0)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        touch[i] = Input.GetTouch(i);

                        if (touch[i].phase == TouchPhase.Began && touch[i].position.x < Screen.width / 2)
                        {
                            SFX[2].Play();
                            GetComponent<Rigidbody2D>().velocity = new Vector2(0, JumpPower);
                            break;
                        }
                    }
                }

                if (Time.time > nextfire && attack && !die)
                {
                    SFX[1].Play();
                    nextfire = Time.time + firerate;
                    Instantiate(Fire, FirePos.position, FirePos.rotation);
                }
            }

            // PC

            else
            {
                if (Input.GetButtonDown("Jump") && !die)
                {
                    SFX[2].Play();
                    GetComponent<Rigidbody2D>().velocity = new Vector2(0, JumpPower);
                }

                if (Input.GetMouseButton(0) && Time.time > nextfire && !die)
                {
                    SFX[1].Play();
                    nextfire = Time.time + firerate;
                    Instantiate(Fire, FirePos.position, FirePos.rotation);
                }

                if (Input.GetMouseButtonDown(1) && Mana >= 5f && !die)
                {
                    SFX[1].Play();
                    Mana -= 5f;
                    Instantiate(Skill, FirePos.position, FirePos.rotation);
                }
            }
        }
    }
}
