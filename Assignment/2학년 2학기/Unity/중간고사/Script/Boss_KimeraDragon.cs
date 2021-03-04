using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Boss_KimeraDragon : MonoBehaviour
{
    public Transform[] SkillPos;
    public Transform[] RainPos;
    public Transform ShotPos, ShotPos2, ShotPos3;

    public GameObject Bolt, Arrow, Strom, Shot;
    public GameObject[] Shield;

    public Slider HPber;
    public AudioSource Skill_SFX;

    bool skill_active, once, once2;
    bool shield_active;

    public int life;
    float rate;

    // Start is called before the first frame update
    void Start()
    {
        skill_active = true;
        once = false;
        once2 = false;

        shield_active = false;

        StartCoroutine(RandomSkill());

        rate = 0f;
        life = 13000;

        HPber.maxValue = life;
        HPber.minValue = 0;

        PlayerPrefs.SetInt("NowStage", 5);
    }

    IEnumerator RandomSkill()
    {
        int num = 1;

        while (true)
        {
            if (!skill_active)
            {
                num = Random.Range(0, 5);

                switch (num)
                {
                    case 0:
                        Debug.Log("bolt");
                        StartCoroutine(BoltActive());
                        break;

                    case 1:
                        Debug.Log("Rain");
                        StartCoroutine(RainActive());
                        break;

                    case 2:
                        Debug.Log("Shot");
                        StartCoroutine(ShotActive());
                        break;

                    case 3:
                        Debug.Log("Arrow");
                        StartCoroutine(ArrowActive());
                        break;

                    default:
                        Debug.Log("Shield");
                        if (!shield_active)
                        {
                            StartCoroutine(ShieldActive());
                        }
                        break;
                }
                yield return new WaitForSeconds(4f);
            }
            yield return new WaitForSeconds(0.5f);
        }
    }

    IEnumerator ArrowActive()
    {
        skill_active = true;
         
        for (int i = 0; i < 3; i++)
        {
            Skill_SFX.Play();
            Instantiate(Arrow, SkillPos[i].position, Quaternion.Euler(0, 0, 0));
            for (int j = 0; j < 12; j++)
            {
                if (j < 6)
                {
                    Instantiate(Bolt, ShotPos.position, Quaternion.Euler(0, 0, -25 + (20 * j)));
                }
                else if (j > 6 && j < 12)
                {
                    Instantiate(Bolt, ShotPos3.position, Quaternion.Euler(0, 0, 165 - (20 * j)));
                }
            }
            yield return new WaitForSeconds(1f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator ShotActive()
    {
        skill_active = true;

        Instantiate(Shot, ShotPos.position, Quaternion.Euler(0, 0, 0));
        Instantiate(Shot, ShotPos3.position, Quaternion.Euler(0, 0, 0));

        for (int i = 0; i < 4; i++)
        {
            Skill_SFX.Play();

            Instantiate(Strom, ShotPos2.position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(0.5f);

            for (int j = 0; j < 12; j++)
            {
                if (j < 6)
                {
                    Instantiate(Bolt, SkillPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
                }
                else if (j > 6 && j < 12)
                {
                    Instantiate(Bolt, SkillPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, 165 - (20 * j)));
                }
                yield return new WaitForSeconds(0.1f);
            }
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator RainActive()
    {
        skill_active = true;

        for (int i = 0; i < 3; i++)
        {
            Skill_SFX.Play();

            for (int j = 0; j < 18; j++)
            {
                Instantiate(Bolt, RainPos[Random.Range(0, 1)].position, Quaternion.Euler(0, 0, 45 + (20 * j)));
                Instantiate(Bolt, RainPos[Random.Range(3, 4)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            Instantiate(Strom, ShotPos.position, Quaternion.Euler(0, 0, 0));
            Instantiate(Bolt, ShotPos2.position, Quaternion.Euler(0, 0, 0));
            Instantiate(Strom, ShotPos3.position, Quaternion.Euler(0, 0, 0));
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator BoltActive()
    {
        skill_active = true;

        for (int i = 0; i < 4; i++)
        {
            Skill_SFX.Play();

            Instantiate(Strom, ShotPos.position, Quaternion.Euler(0, 0, 0));
            Instantiate(Strom, ShotPos3.position, Quaternion.Euler(0, 0, 0));

            for (int j = 0; j < 18; j++)
            {
                if (j < 6)
                {
                    Instantiate(Bolt, SkillPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
                }
                else if (j > 6 && j < 12)
                {
                    Instantiate(Bolt, SkillPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, 165 - (20 * j)));
                }
                else if (j > 12)
                {
                    Instantiate(Bolt, SkillPos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, 45 + (20 * j)));
                }
                yield return new WaitForSeconds(0.1f);
            }
            yield return new WaitForSeconds(0.5f);
            Instantiate(Arrow, RainPos[Random.Range(0, 5)].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(1f);

            yield return new WaitForSeconds(1f);
        }
        yield return new WaitForSeconds(1f);

        skill_active = false;
    }

    IEnumerator ShieldActive()
    {
        int num = Random.Range(0, 2);
        shield_active = true;
        Shield[num].SetActive(true);
        yield return new WaitForSeconds(10f);
        Shield[num].SetActive(false);
        shield_active = false;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            life -= FireShot.Attack;

            if (life <= 0)
            {
                GameManager.score += 80000;
                SceneManager.LoadScene("Win");
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            life -= SkillShot.attck;

            if (life <= 0)
            {
                GameManager.score += 80000;
                SceneManager.LoadScene("Win");
            }
        }
    }

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

                if (rate < 0.4f && transform.position.y <= 3.6f)
                {
                    transform.Translate(0, 3.5f * Time.deltaTime, 0);
                }
                else if (rate > 0.4f && transform.position.y >= -2.12f)
                {
                    transform.Translate(0, -3.5f * Time.deltaTime, 0);
                }
                if (rate > 0.8f)
                {
                    rate = 0f;
                }

                if (rate < 0.2f && transform.position.x >= 4.5f)
                {
                    transform.Translate(-2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.2f && rate < 0.4f && transform.position.x <= 6.5f)
                {
                    transform.Translate(2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.4f && rate < 0.6f && transform.position.x <= 8.5f)
                {
                    transform.Translate(2f * Time.deltaTime, 0, 0);
                }
                else if (rate > 0.6f && rate < 0.8f && transform.position.x >= 6.5f)
                {
                    transform.Translate(-2f * Time.deltaTime, 0, 0);
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
