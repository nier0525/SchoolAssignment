using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fireball_bolt_impactSfx : MonoBehaviour {
    public GameObject impactSfx;

    GameObject Sfx;
	// Use this for initialization
	void Start () {
        Sfx = Instantiate(impactSfx) as GameObject;
        Sfx.transform.parent = this.transform;
    }
}
