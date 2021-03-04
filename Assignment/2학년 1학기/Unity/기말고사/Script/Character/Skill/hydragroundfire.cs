using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hydragroundfire : MonoBehaviour {
    public GameObject firemid;
    public GameObject fireend;
    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;
    private bool create;

    // Use this for initialization
    void Start () {
        create = true;
        damage = Random.Range(min_damage, max_damage);
        Debug.Log(damage);
    }

    // Update is called once per frame
    void Update () {

        if (this.gameObject.name == "firestart(Clone)")
        {
            if (create)
            {
                StartCoroutine("blaze_mid");
                create = false;
            }
            Destroy(this.gameObject, 0.3f);
        }

        if(this.gameObject.name == "firemid(Clone)")
        {
            if(create)
            {
                StartCoroutine("blaze_end");
                create = false;
            }
            Destroy(this.gameObject, 10.0f);
        }

        if(this.gameObject.name == "fireend(Clone)")
        {
            Destroy(this.gameObject, 0.58f);
        }
    }

    IEnumerator blaze_mid()
    {
        yield return new WaitForSeconds(0.25f);
        GameObject item = Instantiate(firemid) as GameObject;
        item.transform.position = new Vector2(transform.position.x, transform.position.y+0.15f);
    }

    IEnumerator blaze_end()
    {
        yield return new WaitForSeconds(9.85f);
        GameObject item = Instantiate(fireend) as GameObject;
        item.transform.position = this.transform.position;
    }
}
