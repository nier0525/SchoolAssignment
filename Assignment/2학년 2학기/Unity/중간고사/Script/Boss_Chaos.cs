using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Boss_Chaos : MonoBehaviour
{
    public GameObject ChaosBolt, ChaosArrow, ChaosShot, Shield;
    public Transform[] SkillPos;

    public Slider HPber;
    public AudioSource Skill_SFX;

    bool skill_active, once;
    bool shield_active;

    public int life;
    float rate;

    // Start is called before the first frame update
    void Start()
    {
        skill_active = true;
        once = false;

        shield_active = false;

        StartCoroutine(RandomSkill());

        rate = 0f;
        life = 8500;

        HPber.maxValue = life;
        HPber.minValue = 0;

        PlayerPrefs.SetInt("NowStage", 3);
    }

    IEnumerator RandomSkill()
    {
        int num = 0;

        while (true)
        {
            if (!skill_active)
            {
                num = Random.Range(0, 5);

                switch (num)
                {
                    case 0:
                        Debug.Log("ChaosShot");
                        StartCoroutine(ChaosShotActive());
                        break;

                    case 1:
                        Debug.Log("ChaosBolt");
                        StartCoroutine(ChaosBoltActive());
                        break;

                    case 2:
                        Debug.Log("Chaos Rain");
                        StartCoroutine(ChaosRainActive());
                        break;

                    case 3:
                        if (!shield_active)
                        {
                            Debug.Log("Shiled");
                            StartCoroutine(ShieldActive());
                        }
                        break;

                    default:
                        Debug.Log("ChaosArrow");
                        StartCoroutine(ChaosArrowActive());
                        break;
                }
                yield return new WaitForSeconds(4f);
            }
            yield return new WaitForSeconds(1f);
        }
    }

    IEnumerator ShieldActive()
    {
        shield_active = true;
        Shield.SetActive(true);
        yield return new WaitForSeconds(10f);
        Shield.SetActive(false);
        shield_active = false;
    }

    IEnumerator ChaosShotActive()
    {
        skill_active = true;

        for (int i = 0; i < 3; i++)
        {
            Skill_SFX.Play();
            Instantiate(ChaosShot, SkillPos[i].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(2f);
        }

        skill_active = false;
    }

    IEnumerator ChaosBoltActive()
    {
        skill_active = true;

        for (int i = 0; i < 5; i++)
        {
            Skill_SFX.Play();

            for (int j = 0; j < 18; j++)
            {
                Instantiate(ChaosBolt, SkillPos[Random.Range(0, 3)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            Instantiate(ChaosArrow, SkillPos[1].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(1.5f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator ChaosRainActive()
    {
        skill_active = true;

        for (int i = 0; i < 5; i++)
        {
            Skill_SFX.Play();

            for (int j = 0; j < 5; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    Instantiate(ChaosBolt, SkillPos[k + 4].position, Quaternion.Euler(0, 0, 45 + (30 * j)));
                }
            }
            yield return new WaitForSeconds(2f);
        }

        skill_active = false;
    }

    IEnumerator ChaosArrowActive()
    {
        skill_active = true;

        for (int i = 0; i < 3; i++)
        {
            Skill_SFX.Play();

            Instantiate(ChaosArrow, SkillPos[i].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(1f);
        }

        skill_active = false;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            life -= FireShot.Attack;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Story") == 1)
                {
                    PlayerPrefs.SetInt("Story", 2);
                }
                GameManager.score += 25000;
                SceneManager.LoadScene("Win");
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            life -= SkillShot.attck;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Story") == 1)
                {
                    PlayerPrefs.SetInt("Story", 2);
                }
                GameManager.score += 25000;
                SceneManager.LoadScene("Win");
            }
        }
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        HPber.value = life;

        if (transform.position.x >= 5.9f)
        {
            transform.Translate(-4f * Time.deltaTime, 0, 0);
        }
        else
        {
            rate += Time.deltaTime / 4;

            if (rate < 0.5f && transform.position.y <= 2.5f)
            {
                transform.Translate(0, 2f * Time.deltaTime, 0);
            }
            else if (rate > 0.5f && transform.position.y >= -1.54f)
            {
                transform.Translate(0, -2f * Time.deltaTime, 0);
            }
            if (rate > 1f)
            {
                rate = 0f;
            }



            if (!once)
            {
                skill_active = false;
                once = true;
            }
        }
    }
}
