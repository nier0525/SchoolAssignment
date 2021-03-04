using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.AI;

using UnityEngine.SceneManagement;

public class HP_Status : MonoBehaviour
{
    public Slider HPvar;
    public AudioSource Die, Hit_Sound;

    public GameObject Blood;

    public float max_hp;
    private float current_hp;

    public bool alive = true;
    public bool hit = false;

    bool done;
    
    GameManager GM;

    // Start is called before the first frame update
    void Start()
    {
        done = false;

        current_hp = max_hp;

        HPvar.maxValue = max_hp;
        HPvar.minValue = 0;

        HPvar.value = current_hp;

        GM = GameObject.Find("GameManager").GetComponent<GameManager>();

        if (Blood != null)
            Blood.SetActive(false);
    }

    public void Hit_Damage(float damage)
    {
        hit = true;

        if (alive)
        {
            current_hp -= damage;
            HPvar.value = current_hp;

            if (current_hp <= 0)
            {
                Debug.Log("쥬금");
                if (Die != null)
                    Die.Play();

                if (gameObject.tag == "Tower")
                {
                    GM.TowerCheak(gameObject);
                    Destroy(gameObject);
                }

                else if (gameObject.tag == "Player")
                {
                    SceneManager.LoadScene("Lose");
                }

                else  // Monster
                {
                    GetComponent<Animator>().SetBool("dead", true);
                    GetComponent<NavMeshAgent>().enabled = false;

                    CapsuleCollider[] obj = gameObject.GetComponentsInChildren<CapsuleCollider>();

                    if (obj != null)
                    {
                        for (int i = 0; i < obj.Length; i++)
                            obj[i].isTrigger = true;
                    }
                    else
                        Debug.Log("Not Search");

                    GM.UpdateScore(max_hp * 0.5f);
                    Destroy(gameObject, 10f);
                }

                alive = false;
            }

            else
            {
                if (gameObject.tag == "Player")
                {
                    if (!done)
                        StartCoroutine(Player_Blood());

                    if (Hit_Sound != null)
                        Hit_Sound.Play();
                }

                else
                {
                    if (Hit_Sound != null)
                        Hit_Sound.Play();
                }
            }
        }
    }

    IEnumerator Player_Blood()
    {
        if (Blood == null) {
            done = false;
            yield return new WaitForSeconds(0f);
        }

        done = true;
        Blood.SetActive(true);
        yield return new WaitForSeconds(0.5f);
        Blood.SetActive(false);
        done = false;

        yield return new WaitForSeconds(0f);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
