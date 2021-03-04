using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Boss_Ice : MonoBehaviour
{
    public GameObject IceShot, IceLance, IceBolt, Shield;
    public Transform [] IceShot_pos;
    public Slider HPbar;

    public AudioSource Skill_SFX;

    bool skill_active, once;
    bool Shield_active;

    public int life;

    // Start is called before the first frame update
    void Start()
    {
        skill_active = true;
        once = false;
        Shield_active = false;

        StartCoroutine(RandomSkill());

        life = 7500;

        HPbar.maxValue = life;
        HPbar.minValue = 0;

        PlayerPrefs.SetInt("NowStage", 1);
    }

    IEnumerator RandomSkill()
    {
        int num = 0;

        while(true)
        {
            if (!skill_active)
            {
                num = Random.Range(0, 4);

                switch (num)
                {
                    case 0:
                        Debug.Log("IceShot");
                        StartCoroutine(IceShotActive());
                        break;

                    case 1:
                        Debug.Log("IceBolt");
                        StartCoroutine(IceBoltActive());
                        break;

                    case 2:
                        Debug.Log("IceLance");
                        StartCoroutine(IceLanceActive());
                        break;

                    default:
                        if (!Shield_active)
                        {
                            Debug.Log("Shield");
                            StartCoroutine(ShieldActive());
                        }
                        break;
                }
                yield return new WaitForSeconds(5f);
            }
            yield return new WaitForSeconds(1f);
        }
    }

    IEnumerator ShieldActive()
    {
        Shield_active = true;
        Shield.SetActive(true);
        yield return new WaitForSeconds(8f);
        Shield.SetActive(false);
        Shield_active = false;
    }

    IEnumerator IceShotActive()
    {
        skill_active = true;

        for (int i = 0; i < IceShot_pos.Length; i++)
        {
            Skill_SFX.Play();

            Instantiate(IceShot, IceShot_pos[i]);
            yield return new WaitForSeconds(2f);
        }

        skill_active = false;
    }

    IEnumerator IceLanceActive()
    {
        skill_active = true;
        Skill_SFX.Play();

        for (int i = 0; i < IceShot_pos.Length; i++)
        {
            if (i != 1)
            {
                Skill_SFX.Play();

                Instantiate(IceLance, IceShot_pos[i]);
                yield return new WaitForSeconds(5f);
            }
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator IceBoltActive()
    {
        skill_active = true;
        Skill_SFX.Play();

        for (int i = 0; i < 7; i++)
        {
            Skill_SFX.Play();

            for (int j = 0; j < 18; j++)
            {
                Instantiate(IceBolt, IceShot_pos[Random.Range(0, 2)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            }
            yield return new WaitForSeconds(1.5f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Bullet")
        {
            life -= FireShot.Attack;
            
            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Stage") == 0)
                {
                    PlayerPrefs.SetInt("Stage", 1);
                }

                GameManager.score += 10000;
                SceneManager.LoadScene("Win");
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            life -= SkillShot.attck;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Stage") == 0)
                {
                    PlayerPrefs.SetInt("Stage", 1);
                }

                GameManager.score += 10000;
                SceneManager.LoadScene("Win");
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        HPbar.value = life;

        if (transform.position.x >= 6f)
        {
            transform.Translate(-4f * Time.deltaTime, 0, 0);
        }
        else
        {
            if (!once)
            {
                skill_active = false;
                once = true;
            }
        }
    }
}
