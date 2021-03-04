using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HepstoAI : MonoBehaviour
{
    public GameObject player, SkillFire;
    public GameObject Shadow;
    public SkillDatabase SkillDamege;
    public Slider HPbar;
    public Text Name;

    public AudioSource Attack, Death, GetHit, Skill;

    Vector2 player_pos;
    PathFinding PfScript;

    HepstoMotion demon_m;
    HepstoStatus demon_s;

    IEnumerator coroutineisAttak;

    public bool death = false;
    public bool DemonHp = false;
    public bool isSkill = false;
    bool SunBbbng = false;
    bool move = false;
    bool Exit = false;

    bool sound = true;
    bool player_die;

    Color a;

    // Start is called before the first frame update
    void Start()
    {
        PfScript = GetComponent<PathFinding>();

        demon_m = GetComponent<HepstoMotion>();
        demon_s = GetComponent<HepstoStatus>();
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
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Kinematic;
            gameObject.GetComponent<Rigidbody2D>().useFullKinematicContacts = true;

            yield return new WaitForSeconds(0.1f);

            if (!death && !player_die)
            {
                Shadow.GetComponent<HepstoShadowMotion>().Attak(player_pos);
                demon_m.Attak(player_pos);

                yield return new WaitForSeconds(0.5f);
                Attack.Play();
                AttakDamege();
                yield return new WaitForSeconds(0.3f);
                demon_m.ani.SetBool("Attak", false);
                demon_m.ani.SetBool("Work", false);
                if (Exit)
                {
                    break;
                }
                yield return new WaitForSeconds(1f);
            }
        }
        yield return new WaitForSeconds(0.5f);
        gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        demon_m.isAtt = false;
        move = false;
    }

    IEnumerator UsingSkill()
    {
        Debug.Log("코루틴 시작");
        Skill.Play();
        Color c = new Color(1, 0, 0);
        yield return new WaitForSeconds(0.3f);
        gameObject.GetComponent<SpriteRenderer>().color = c;
        a = gameObject.GetComponent<SpriteRenderer>().color;
        SkillFire.SetActive(true);

        demon_s.Att *= 2;
        demon_s.Def += 25;
        demon_m.work_speed += 0.4f;
    }

    IEnumerator HitSound()
    {
        sound = false;
        GetHit.Play();
        if (!death && Exit)
            demon_m.ani.SetTrigger("Hit");
        yield return new WaitForSeconds(1f);
        sound = true;
    }

    IEnumerator DeathWait()
    {
        Shadow.SetActive(false);
        SkillFire.SetActive(false);
        GetComponent<CircleCollider2D>().isTrigger = true;
        Death.Play();
        gameObject.GetComponent<SpriteRenderer>().sortingOrder = 0;
        gameObject.GetComponent<CircleCollider2D>().enabled = false;
        gameObject.GetComponent<CapsuleCollider2D>().enabled = false;
        Debug.Log("death");
        Quest.Next_Quest();
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
                if (Exit)
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
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.Blaze;
                    }
                }
            }

            if (col.gameObject.tag == "Skill_icebolt")
            {
                demon_s.hp -= SkillDamege.Icebolt;
                if (Exit)
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
                if (Exit)
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
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.M_fire;
                    }
                }
            }

            if (col.gameObject.tag == "Skill_firewall")
            {
                if (Exit)
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
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.Firewall;
                    }
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

        if (collision.gameObject.name == "Sorceress")
        {
            if (isSkill)
            {
                player.GetComponent<PlayerState>().hp -= 1;
            }
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

    }

    private void OnCollisionEnter2D(Collision2D col)
    {
        if (!death)
        {
            if (col.gameObject.tag == "Skill_meteor")
            {
                demon_s.hp -= SkillDamege.Meteo;
                if (Exit)
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
        
            if (col.gameObject.tag == "Skill_blizzard")
            {
                demon_s.hp -= SkillDamege.Blizzard;
                if (Exit)
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
                if (Exit)
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
                if (Exit)
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
                if (Exit)
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

    }

    private void OnMouseOver()
    {
        if (!death)
        {
            if (!isSkill)
                gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1);
            if (isSkill)
                gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 0, 0, 1);

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
            PlayerControl.HitSound = true;
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

        if ((dir >= 0.8f && dir < 6f || SunBbbng) && !player_die)
        {
            PfScript.Raycasting(player_pos);

            Exit = true;
            move = false;

            if (!death && !demon_m.isAtt)
            {
                if (!PfScript.isMove)
                    demon_m.Work(player_pos);
                else
                {
                    demon_m.ani.SetBool("Work", true);

                    PfScript.MoveToNode(demon_m.work_speed, demon_m.ani, 1 , 0);
                }
            }

            else
            {
                demon_m.ani.SetBool("Work", false);
            }

            if (demon_s.hp <= 1000 && !isSkill)
            {
                StartCoroutine(UsingSkill());
                isSkill = true;
            }
        }

        if (dir < 0.8f)
        {
            demon_m.ani.SetBool("Work", false);

            if (!move)
            {
                PfScript.isMove = false;
                PfScript.isWall = false;

                Exit = false;
                move = true;
                coroutineisAttak = AttakDelay();
                StartCoroutine(coroutineisAttak);
            }

            SunBbbng = false;
            demon_m.isAtt = true;
        }

        if (dir > 6f && !death && !player_die && !SunBbbng)
        {
            PfScript.isMove = false;
            PfScript.isWall = false;

            demon_m.ani.SetBool("Work", false);
            demon_m.isWork = false;
        }

        if (player_die)
        {
            demon_m.isAtt = false;
            demon_m.isWork = false;
            demon_m.ani.SetBool("Work", false);
            //StopCoroutine(coroutineisAttak);
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        }
    }
}

