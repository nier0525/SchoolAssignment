using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class firewall : MonoBehaviour {
    public GameObject firewall_mid;
    public GameObject firewall_end;
    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;

    private bool create;

    // Use this for initialization
    void Start()
    {
        create = true;
        damage = Random.Range(min_damage, max_damage);
        Debug.Log(damage);
    }

    // Update is called once per frame
    void Update()
    {
        if (this.gameObject.name == "firewall_start(Clone)")
        {
            if (create)
            {
                StartCoroutine("Firewall_mid");
                create = false;
            }
            Destroy(this.gameObject, 1.0f);
        }

        if (this.gameObject.name == "firewall_mid(Clone)")
        {
            if (create)
            {
                StartCoroutine("Firewall_end");
                create = false;
            }
            Destroy(this.gameObject, 8.0f);
        }

        if (this.gameObject.name == "firewall_end(Clone)")
        {
            Destroy(this.gameObject, 1.1f);
        }
    }

    IEnumerator Firewall_mid()
    {
        yield return new WaitForSeconds(0.85f);
        GameObject item = Instantiate(firewall_mid) as GameObject;
        item.transform.position = new Vector2(transform.position.x, transform.position.y);
    }

    IEnumerator Firewall_end()
    {
        yield return new WaitForSeconds(7.85f);
        GameObject item = Instantiate(firewall_end) as GameObject;
        item.transform.position = this.transform.position;
    }
}
