using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Boss_FallenKing : MonoBehaviour
{
    public Transform[] SkillPos;
    public Transform[] RainPos;
    public Transform ShotPos, ShotPos2;

    public GameObject Bolt, Arrow, Strom, Shot, Reverce;

    public Slider HPber;
    public AudioSource Skill_SFX;

    bool skill_active, once, once2;
    bool reverce_active;

    public int life;
    float rate;

    // Start is called before the first frame update
    void Start()
    {
        skill_active = true;
        once = false;
        once2 = false;
       reverce_active = false;

        StartCoroutine(RandomSkill());

        rate = 0f;
        life = 10500;

        HPber.maxValue = life;
        HPber.minValue = 0;
    }

    IEnumerator RandomSkill()
    {
        int num = 0;

        while(true)
        {
            if (!skill_active)
            {
                num = Random.Range(0, 7);
                
                switch(num)
                {
                    case 0:
                        Debug.Log("Bolt");
                        StartCoroutine(BoltActive());
                        break;

                    case 1:
                        Debug.Log("Strom");
                        StartCoroutine(StromActive());
                        break;

                    case 2:
                        Debug.Log("Rain");
                        StartCoroutine(RainActive());
                        break;

                    case 3:
                        Debug.Log("Shot");
                        StartCoroutine(ShotActive());
                        break;

                    case 4:
                        if (!reverce_active)
                        {
                            Debug.Log("Reverse");
                            StartCoroutine(ReverseActive());
                        }
                        break;

                    default:
                        Debug.Log("Arrow");
                        StartCoroutine(ArrowActive());
                        break;
                }

                yield return new WaitForSeconds(4f);
            }
            yield return new WaitForSeconds(0.5f);
        }
    }

    IEnumerator BoltActive()
    {
        skill_active = true;

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                Instantiate(Bolt, SkillPos[Random.Range(0, 4)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            Instantiate(Arrow, SkillPos[4].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(1.5f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator StromActive()
    {
        skill_active = true;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                Instantiate(Bolt, SkillPos[Random.Range(0, 4)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            Instantiate(Strom, SkillPos[4].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(2f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator RainActive()
    {
        skill_active = true;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                Instantiate(Bolt, RainPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, 45 + (20 * j)));
                Instantiate(Bolt, RainPos[Random.Range(3, 5)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            yield return new WaitForSeconds(2f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator ShotActive()
    {
        skill_active = true;

        for (int i = 0; i < 2; i++)
        {
            Instantiate(Shot, ShotPos.position, Quaternion.Euler(0, 0, 0));
            Instantiate(Shot, ShotPos2.position, Quaternion.Euler(0, 0, 0));

            for (int j = 0; j < 18; j++)
            {
                Instantiate(Bolt, RainPos[Random.Range(0, 1)].position, Quaternion.Euler(0, 0, 45 + (20 * j)));
                Instantiate(Bolt, RainPos[Random.Range(3, 4)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }

            for (int k = 0; k < 2; k++)
            {
                Instantiate(Strom, SkillPos[4].position, Quaternion.Euler(0, 0, 0));
                yield return new WaitForSeconds(3f);
            }
            yield return new WaitForSeconds(2f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator ReverseActive()
    {
        reverce_active = true;
        Reverce.SetActive(true);
        yield return new WaitForSeconds(10f);
        Reverce.SetActive(false);
        reverce_active = false;
    }

    IEnumerator ArrowActive()
    {
        skill_active = true;

        for (int j = 0; j < 3; j++)
        {
            Instantiate(Arrow, RainPos[Random.Range(0, 5)].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(1f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            life -= FireShot.Attack;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Stage") == 3)
                {
                    PlayerPrefs.SetInt("Story", 3);
                    PlayerPrefs.SetInt("Stage", 4);
                }

                GameManager.score += 50000;
                SceneManager.LoadScene("Win");
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            life -= SkillShot.attck;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Stage") == 3)
                {
                    PlayerPrefs.SetInt("Story", 3);
                    PlayerPrefs.SetInt("Stage", 4);
                }

                GameManager.score += 50000;
                SceneManager.LoadScene("Win");
            }
        }
    }
    
    // Update is called once per frame
    void FixedUpdate()
    {
        HPber.value = life;

        if (transform.position.x >= 6.5f && !once2)
        {
            transform.Translate(-4f * Time.deltaTime, 0, 0);
        }

        else
        {
            {
                rate += Time.deltaTime / 4;
                once2 = true;

                if (rate < 0.5f && transform.position.y <= 3.08f)
                {
                    transform.Translate(0, 3f * Time.deltaTime, 0);
                }
                else if (rate > 0.5f && transform.position.y >= -2.86f)
                {
                    transform.Translate(0, -3f * Time.deltaTime, 0);
                }
                if (rate > 1f)
                {
                    rate = 0f;
                }

                if (rate < 0.25f && transform.position.x >= 4.5f)
                {
                    transform.Translate(-2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.25f && rate < 0.5f && transform.position.x <= 6.5f)
                {
                    transform.Translate(2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.5f && rate < 0.75f && transform.position.x >= 4.5f)
                {
                    transform.Translate(-2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.75f && rate < 1f && transform.position.x <= 6.5f)
                {
                    transform.Translate(2f * Time.deltaTime, 0, 0);
                }
            }

            if (!once)
            {
                skill_active = false;
                once = true;
            }
        }
    }
}
