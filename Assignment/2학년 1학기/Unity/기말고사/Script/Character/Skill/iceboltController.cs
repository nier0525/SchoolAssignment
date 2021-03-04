using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class iceboltController : MonoBehaviour {
    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;
    public float Angle;
    Animator Anim;

    private void Start()
    {
        Anim = GetComponent<Animator>();
        Anim.SetFloat("Angle",Angle);
        damage = Random.Range(min_damage, max_damage);
    }

    // Update is called once per frame
    void Update () {
        Destroy(this.gameObject, 1.5f);
    }
}
