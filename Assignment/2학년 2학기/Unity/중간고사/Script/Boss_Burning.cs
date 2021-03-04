using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Boss_Burning : MonoBehaviour
{
    public GameObject FireBolt, FireShot_s, FireLance, FireArrow;

    public Transform[] SkillPos;
    public Slider HPber;
    public AudioSource SFX;

    bool skill_active, once;

    public int life;

    // Start is called before the first frame update
    void Start()
    {
        skill_active = true;
        once = false;

        StartCoroutine(RandomSkill());

        life = 9000;
        HPber.maxValue = life;
        HPber.minValue = 0;

        PlayerPrefs.SetInt("NowStage", 2);
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
                        Debug.Log("FireShot");
                        StartCoroutine(FireShotActive());
                        break;

                    case 1:
                        Debug.Log("FireBolt");
                        StartCoroutine(FireBoltActive());
                        break;

                    case 2:
                        Debug.Log("Hell in a Sell");
                        StartCoroutine(HellActive());
                        break;

                    default:
                        Debug.Log("Fire Arrow");
                        StartCoroutine(FireArrowActive());
                        break;
                }
                yield return new WaitForSeconds(4f);
            }
            yield return new WaitForSeconds(1f);
        }
    }

    IEnumerator FireShotActive()
    {
        skill_active = true;

        for (int i = 0; i < 3; i++)
        {
            SFX.Play();

            Instantiate(FireShot_s, SkillPos[1].position, Quaternion.Euler(0, 0, 0));
            for (int j = 0; j < 18; j++)
            {
                Instantiate(FireBolt, SkillPos[4].position, Quaternion.Euler(0, 0, -25 + (10 * j)));
            }

            yield return new WaitForSeconds(3f);
        }

        skill_active = false;
    }

    IEnumerator FireBoltActive()
    {
        skill_active = true;

        for (int i = 0; i < 6; i++)
        {
            SFX.Play();

            for (int j = 0; j < 18; j++)
            {
                for (int z = 0; z < SkillPos.Length / 2; z++)
                {
                    Instantiate(FireBolt, SkillPos[Random.Range(0, 5)].position, Quaternion.Euler(0, 0, -25 + (20 * j)));
                }
            }
            Instantiate(FireArrow, SkillPos[1].position, Quaternion.Euler(0, 0, 0));
            yield return new WaitForSeconds(2f);
        }

        yield return new WaitForSeconds(1f);
        skill_active = false;
    }

    IEnumerator FireArrowActive() {
        skill_active = true;

        SFX.Play();
        Instantiate(FireArrow, SkillPos[Random.Range(0, 5)].position, Quaternion.Euler(0, 0, 0));
        yield return new WaitForSeconds(0.1f);

        skill_active = false;
    }

    IEnumerator HellActive()
    {
        skill_active = true;

        for (int i = 0; i < 3; i++)
        {
            SFX.Play();

            Instantiate(FireLance, SkillPos[i]);

            for (int j = 0; j < 5; j++)
            {
                SFX.Play();

                for (int k = 0; k < 18; k++)
                {
                    Instantiate(FireBolt, SkillPos[Random.Range(0, 5)].position, Quaternion.Euler(0, 0, -25 + (20 * k)));
                }
                yield return new WaitForSeconds(2f);
            }
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
                if (PlayerPrefs.GetInt("Stage") == 1)
                {
                    PlayerPrefs.SetInt("Stage", 2);
                }

                GameManager.score += 15000;
                SceneManager.LoadScene("Win");
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            life -= SkillShot.attck;

            if (life <= 0)
            {
                if (PlayerPrefs.GetInt("Stage") == 1)
                {
                    PlayerPrefs.SetInt("Stage", 2);
                }

                GameManager.score += 15000;
                SceneManager.LoadScene("Win");
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        HPber.value = life;

        if (transform.position.x >= 5.5f)
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
