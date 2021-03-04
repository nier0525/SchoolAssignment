using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MegaDemonControl : MonoBehaviour
{
    public GameObject player;
    public GameObject Shadow;
    public SkillDatabase SkillDamege;
    public Slider HPbar;
    public Text Name;

    public AudioSource Attack, Death, GetHit;

    Vector2 player_pos;

    DemonMotion demon_m;
    MegaDemonStatus demon_s;

    IEnumerator coroutineisAttak;
    public bool death = false;
    public bool DemonHp = false;
    bool SunBbbng = false;
    bool move = false;
    bool Exit = false;

    bool sound = true;
    bool player_die;

    Color a;

    // Start is called before the first frame update
    void Start()
    {
        demon_m = GetComponent<DemonMotion>();
        demon_s = GetComponent<MegaDemonStatus>();
        GetComponent<CapsuleCollider2D>().isTrigger = false;
        a = gameObject.GetComponent<SpriteRenderer>().color;
        HPbar.gameObject.SetActive(false);
        Name.gameObject.SetActive(false);
        HPbar.maxValue = demon_s.hp;
    }

    private void Awake()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }

    IEnumerator MonsterEnter()
    {
        while (true)
        {
            yield return new WaitForSeconds(1f);
            demon_m.ani.SetBool("Work", false);
            demon_m.work_speed = 0f;
        }
    }

    IEnumerator AttakDelay()
    {
        while (!Exit)
        {
            yield return new WaitForSeconds(0.5f);

            if (!death && !player_die)
            {
                demon_m.Attak(player_pos);

                yield return new WaitForSeconds(0.7f);
                Attack.Play();
                AttakDamege();
                move = false;
                yield return new WaitForSeconds(0.3f);
                demon_m.ani.SetBool("Attak", false);
                demon_m.ani.SetBool("Work", false);
                if (Exit)
                    break;
                yield return new WaitForSeconds(1f);
            }
        }
        yield return new WaitForSeconds(0.5f);
        demon_m.isAtt = false;
    }

    IEnumerator HitSound()
    {
        sound = false;
        GetHit.Play();
        if (!death)
            demon_m.ani.SetTrigger("Hit");
        yield return new WaitForSeconds(1f);
        sound = true;
    }

    IEnumerator DeathWait()
    {
        Shadow.SetActive(false);
        Death.Play();
        if (gameObject.GetComponent<SpriteRenderer>().sortingOrder == 5)
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
        gameObject.GetComponent<CircleCollider2D>().enabled = false;
        gameObject.GetComponent<CapsuleCollider2D>().enabled = false;
        Debug.Log("death");
        yield return null;
        //yield return new WaitForSeconds(6f);
        //DestoryMonster();
    }

    private void OnTriggerEnter2D(Collider2D col)
    {
        if (!death)
        {
            if (col.gameObject.tag == "Skill_blaze")
            {
                demon_s.hp -= SkillDamege.Blaze;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_icebolt")
            {
                demon_s.hp -= SkillDamege.Icebolt;
                SunBbbng = true;
                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_mfire")
            {
                demon_s.hp -= SkillDamege.M_fire;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;
                    demon_m.ani.SetTrigger("Death");

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_firewall")
            {
                demon_s.hp -= SkillDamege.Firewall;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }
        }
    }

    void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Layer_0")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 0;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 0;
        }
        if (collision.gameObject.tag == "Layer_1")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 1;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 1;
        }
        if (collision.gameObject.tag == "Layer_2")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 2;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 2;
        }
        if (collision.gameObject.tag == "Layer_3")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 3;
        }
    }

    void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Layer_0" || collision.gameObject.tag == "Layer_1" ||
            collision.gameObject.tag == "Layer_2" || collision.gameObject.tag == "Layer_3" ||
            collision.gameObject.tag == "Layer_4")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 5;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 4;
        }
    }

    void OnCollisionStay2D(Collision2D col)
    {
        if (col.gameObject.name == "Sorceress")
        {
            demon_m.ani.SetBool("Work", false);
        }
    }

    private void OnCollisionEnter2D(Collision2D col)
    {
        if (!death)
        {
            if (col.gameObject.name == "Sorceress")
            {
                Exit = false;

                SunBbbng = false;
                demon_m.isAtt = true;
                move = true;
                gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Kinematic;
                gameObject.GetComponent<Rigidbody2D>().useFullKinematicContacts = true;

                coroutineisAttak = AttakDelay();
                StartCoroutine(coroutineisAttak);
            }

            if (col.gameObject.tag == "Skill_meteor")
            {
                demon_s.hp -= SkillDamege.Meteo;
                SunBbbng = true;
                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;
                    demon_m.ani.SetTrigger("Death");

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            //if (col.gameObject.tag == "Skill_iceorb")
            //{
            //    demon_s.hp -= SkillDamege.Iceorb;
            //    SunBbbng = true;
            //    if (demon_s.hp <= 0)
            //    {
            //        death = true;
            //        demon_m.isWork = false;

            //        GetComponent<CapsuleCollider2D>().isTrigger = true;
            //        player.GetComponent<PlayerState>().exp += demon_s.exp;
            //        demon_m.ani.SetTrigger("Death");
            //        StartCoroutine(DeathWait());
            //    }

            //    else
            //    {
            //        if (sound)
            //            StartCoroutine(HitSound());
            //    }
            //}

            if (col.gameObject.tag == "Skill_blizzard")
            {
                demon_s.hp -= SkillDamege.Blizzard;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_icestorm")
            {
                demon_s.hp -= SkillDamege.Icestorm;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }

            }

            if (col.gameObject.tag == "Skill_fireball")
            {
                demon_s.hp -= SkillDamege.Fireball;
                Debug.Log(SkillDamege.Fireball);
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_firebolt")
            {
                demon_s.hp -= SkillDamege.Firebolt;
                SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }
        }
    }

    void OnCollisionExit2D(Collision2D col)
    {
        if (col.gameObject.name == "Sorceress")
        {
            move = false;
            Exit = true;
            //StopCoroutine(coroutineisAttak);
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        }
    }

    private void OnMouseOver()
    {
        if (!death)
        {
            gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1);
            HPbar.gameObject.SetActive(true);
            Name.gameObject.SetActive(true);
            DemonHp = true;
        }
    }

    private void OnMouseExit()
    {
        gameObject.GetComponent<SpriteRenderer>().color = a;
        HPbar.gameObject.SetActive(false);
        Name.gameObject.SetActive(false);
        DemonHp = false;
    }

    public void AttakDamege()
    {
        if (!Exit)
        {
            Debug.Log("Attak");
            player.GetComponent<PlayerState>().hp -= demon_s.Att;
        }
    }

    public void DestoryMonster()
    {
        Destroy(this.gameObject);
    }

    // Update is called once per frame
    void Update()
    {
        HPbar.value = demon_s.hp;
        player_pos = player.transform.position;
        float dir = Vector2.Distance(transform.position, player.transform.position);
        player_die = player.GetComponent<PlayerControl>().die;

        if ((dir < 6f || SunBbbng) && !player_die)
        {
            if (!death && !demon_m.isAtt)
            {
                demon_m.Work(player_pos);
            }
            else
            {
                demon_m.ani.SetBool("Work", false);
            }
        }
        else
        {
            demon_m.ani.SetBool("Work", false);
            demon_m.isWork = false;
        }

        if (player_die)
        {
            demon_m.isAtt = false;
            //StopCoroutine(coroutineisAttak);
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        }
    }
}

