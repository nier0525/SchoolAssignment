using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChaosAI : MonoBehaviour
{
    public GameObject player;
    public GameObject Shadow;
    public SkillDatabase SkillDamege;
    public Slider HPbar;
    public Text Name;
    PathFinding PfScript;

    public AudioSource Attack, Death, GetHit;

    Vector2 player_pos;
    Vector2 temp;

    ChaosMotion demon_m;
    ChaosStatus demon_s;

    IEnumerator coroutineisAttak, IdleStart;

    public static float range = 4f;

    public bool death = false;
    public bool DemonHp = false;
    bool SunBbbng = false;
    bool move = false;
    bool Exit = false;
    bool useSkill = false;

    bool Idle = false;

    bool sound = true;
    bool player_die;

    bool cor_attck = false;

    float ray_dis;
    Color a;

    // Start is called before the first frame update
    void Start()
    {
        PfScript = GetComponent<PathFinding>();

        demon_m = GetComponent<ChaosMotion>();
        demon_s = GetComponent<ChaosStatus>();
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

    IEnumerator IdleWalk()
    {
        float index = 1f;

        demon_m.ani.SetBool("Work", false);
        demon_m.isWork = false;

        yield return new WaitForSeconds(1f);

        while (Idle)
        {
            //Debug.Log(index);

            if (index == 0)
            {
                demon_m.ani.SetBool("Work", false);
                demon_m.isWork = false;
            }

            else
            {
                temp = new Vector2(transform.position.x + (Random.Range(-3f, 3f)), transform.position.y + (Random.Range(-3f, 3)));
                demon_m.Work(temp);
            }

            yield return new WaitForSeconds(5f);
            index = Random.Range(0, 3);
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
                Debug.Log("코루틴 시작");

                Shadow.GetComponent<ChaosShadowMotion>().Attak(player_pos);
                demon_m.Attak(player_pos);

                yield return new WaitForSeconds(0.7f);
                Attack.Play();
                AttakDamege();
                yield return new WaitForSeconds(0.3f);
                demon_m.ani.SetBool("Attak", false);
                demon_m.ani.SetBool("Work", false);
                if (Exit)
                {
                    break;
                }
                yield return new WaitForSeconds(0.5f);
            }
        }
        yield return new WaitForSeconds(0.5f);
        gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        demon_m.isAtt = false;
        move = false;
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
                    demon_m.ani.SetTrigger("Death");

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
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
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = -1;
        }
        if (collision.gameObject.tag == "Layer_1")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 1;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 0;
        }
        if (collision.gameObject.tag == "Layer_2")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 2;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 1;
        }
        if (collision.gameObject.tag == "Layer_3")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 2;
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
                Debug.Log(SkillDamege.Fireball);
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
            Color temp = a;
            temp.a = 1;
            gameObject.GetComponent<SpriteRenderer>().color = temp;
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

        float dis = Vector2.Distance(temp, transform.position);
        Vector2 ray_dir = (temp - (Vector2)transform.position) / dis;

        Ray2D ray = new Ray2D(transform.position, ray_dir);
        RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction, ray_dis, 1 << LayerMask.NameToLayer("PathFinding"));

        if (hit)
        {
            if (hit.collider.gameObject.tag == "Wall")
            {
                demon_m.ani.SetBool("Work", false);
                demon_m.isWork = false;
            }
        }

        if ((dir >= 0.8f && dir < range || SunBbbng) && !player_die)
        {
            ray_dis = 0f;

            PfScript.Raycasting(player_pos);

            Idle = false;
            Exit = true;
            move = false;

            if (!death && !demon_m.isAtt)
            {
                if (!PfScript.isMove)
                    demon_m.Work(player_pos);
                else
                {
                    demon_m.ani.SetBool("Work", true);

                    PfScript.MoveToNode(demon_m.work_speed, demon_m.ani, 1 , 1);
                }
            }
        }

        if (dir < 0.8f && !player_die && !death)
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

        if (dir > range && !Idle && !death && !SunBbbng && !player_die)
        {
            ray_dis = 0.5f;

            PfScript.isMove = false;
            PfScript.isWall = false;

            Idle = true;
            IdleStart = IdleWalk();
            StartCoroutine(IdleStart);
        }

        if (player_die && !Idle && !death)
        {
            ray_dis = 0.5f;

            PfScript.isMove = false;
            PfScript.isWall = false;
            Exit = true;

            Idle = true;
            IdleStart = IdleWalk();
            StartCoroutine(IdleStart);

            //StopCoroutine(coroutineisAttak);
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        }
    }
}

