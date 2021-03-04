using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class blizzard : MonoBehaviour {
    public GameObject impact;
    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;
    float times = 0.0f;

    private void Start()
    {
        damage = Random.Range(min_damage, max_damage);
        Debug.Log(damage);
    }



    // Update is called once per frame
    void Update () {

        transform.position -= new Vector3(0, 1.0f * Time.deltaTime, 0);

        times += Time.deltaTime;

        if(times>1.5f)
        {
            GameObject item = Instantiate(impact) as GameObject;
            item.transform.position = new Vector2(transform.position.x + 0.2f, transform.position.y - 0.7f);
            Destroy(this.gameObject);
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Monster")
        {
            GameObject item = Instantiate(impact) as GameObject;
            item.transform.position = new Vector2(transform.position.x+0.2f, transform.position.y - 0.7f);
            Destroy(this.gameObject);
        }
    }
}
