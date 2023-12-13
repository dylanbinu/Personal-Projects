using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class RemyController : MonoBehaviour
{
    private AudioSource remyAudio;

    public AudioClip goldSound;
    public AudioClip goldPouringSound;
    public AudioClip rumbleSound;

    public AudioClip jumpScareSound;
    public AudioClip DyingSound;

    private float offerings = 0;

    public GameObject durgaFlames;
    public GameObject durgaFlame1;
    public GameObject durgaFlame2;
    public GameObject durgaFlame3;
    public GameObject durgaFlame4;
    public GameObject durgaFlame5;
    public GameObject durgaFlame6;
    public GameObject durgaFlame7;
    public GameObject durgaFlame8;

    public GameObject halfWall;
    public GameObject openEntranceDoor;
    public GameObject closedEntranceDoor;
    public GameObject closedExitDoor;
    public GameObject openExitDoor;

    public GameObject puzzleTreasures;
    public GameObject puzzleInstructions;
    public GameObject keyPromptInstructions;

    public GameObject goldPlate;
    public GameObject goldCupOne;
    public GameObject goldCupTwo;
    public GameObject goldCupThree;
    public GameObject goldCandleStickOne;
    public GameObject goldCandleStickTwo;
    public GameObject goldNecklace;
    public GameObject goldIngot;
    public GameObject additionalTreasures;
    public GameObject ruby;
    public GameObject key;

    public GameObject mutantInitial;
    public GameObject mutantIntermediate;
    public GameObject mutantFinal;
    public GameObject mutantDead;

    public GameObject ruinsEntranceInitial;
    public GameObject ruinsEntranceSuccess;
    public GameObject ruinsEntranceEmptySuccess;
    public string gameOverScene;

    // Private Variables

    [Header("Movement")]
    public float walkSpeed = 7.0f;
    public float walkSidewaysSpeed = 4.0f;
    public float runSpeed = 30.0f;
    private float turnSpeed = 100.0f;

    private float horizontalInput;
    private float forwardInput;

    public float groundDrag;

    [Header("Ground Check")]
    public float playerHeight;
    public LayerMask whatIsGround;
    bool grounded;

    public Transform orientation;
    Vector3 forwardMoveDirection;
    Vector3 sidewaysMoveDirection;
    Rigidbody rigidBody;


    // Start is called before the first frame update
    void Start()
    {
        remyAudio = GetComponent<AudioSource>();
        rigidBody = GetComponent<Rigidbody>();
        rigidBody.freezeRotation = true;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        // Receive player input
        horizontalInput = Input.GetAxis("Horizontal");
        forwardInput = Input.GetAxis("Vertical");

        // Determine movement direction based on player input
        forwardMoveDirection = orientation.forward * forwardInput;
        sidewaysMoveDirection = orientation.right * horizontalInput;


        // Ground check
        grounded = Physics.Raycast(transform.position, Vector3.down, playerHeight * 0.5f + 0.2f, whatIsGround);

        // Handle drag
        if (grounded) rigidBody.drag = groundDrag;
        //else rigidBody.drag = 0;



        // Retrieve Remy's velocity
        Vector3 flatVelocity = new Vector3(rigidBody.velocity.x, 0f, rigidBody.velocity.z);

        // Limit Remy's velocity if it exceeds the walkspeed
        if (flatVelocity.magnitude > walkSpeed)
        {
            Vector3 limitedVelocity = flatVelocity.normalized * walkSpeed;
            rigidBody.velocity = new Vector3(limitedVelocity.x, rigidBody.velocity.y, limitedVelocity.z);
        }



        // Perform movements

        if (!Input.GetKey(KeyCode.LeftShift))
        {
            // Make Remy walk
            //transform.Translate(Vector3.forward * Time.deltaTime * walkSpeed * forwardInput);
            rigidBody.AddForce(forwardMoveDirection.normalized * walkSpeed, ForceMode.Force);
        }

        if (Input.GetKey(KeyCode.LeftShift))
        {
            // Make Remy run if player presses Left Shift
            //transform.Translate(Vector3.forward * Time.deltaTime * runSpeed * forwardInput);
            rigidBody.AddForce(forwardMoveDirection.normalized * runSpeed, ForceMode.Force);
        }

        if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.DownArrow))
        {
            // Turn Remy from side to side.
            transform.Rotate(Vector3.up, Time.deltaTime * turnSpeed * horizontalInput);
        }

        if (!Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.DownArrow) && !Input.GetKey(KeyCode.LeftArrow))
        {
            // Move Remy from side to side.
            //transform.Translate(Vector3.right * Time.deltaTime * walkSidewaysSpeed);
            rigidBody.AddForce(sidewaysMoveDirection.normalized * walkSidewaysSpeed, ForceMode.Force);
        }

        if (!Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.DownArrow) && !Input.GetKey(KeyCode.RightArrow))
        {
            // Move Remy from side to side.
            //transform.Translate(Vector3.left * Time.deltaTime * walkSidewaysSpeed);
            rigidBody.AddForce(sidewaysMoveDirection.normalized * walkSidewaysSpeed, ForceMode.Force);
        }

        if (!ruby.activeInHierarchy && mutantDead.activeInHierarchy)
        {
            ruinsEntranceInitial.SetActive(false);
            ruinsEntranceEmptySuccess.SetActive(false);
            ruinsEntranceSuccess.SetActive(true);
        }

        if (ruby.activeInHierarchy && mutantDead.activeInHierarchy)
        {
            ruinsEntranceInitial.SetActive(false);
            ruinsEntranceEmptySuccess.SetActive(true);
        }
    }


    IEnumerator waiterSeven()
    {
        //Wait for 7 seconds
        yield return new WaitForSeconds(7);
    }

    IEnumerator waiterTen()
    {
        //Wait for 7 seconds
        yield return new WaitForSeconds(7);
    }


    public void OnTriggerStay(Collider other)
    {
        if (other.CompareTag("PuzzleInstructions") && Input.GetKey(KeyCode.Space))
        {
            puzzleTreasures.SetActive(true);
            puzzleInstructions.SetActive(true);
            openEntranceDoor.SetActive(false);
            closedEntranceDoor.SetActive(true);

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }

            mutantInitial.SetActive(true);
            StartCoroutine(waiterSeven());
            mutantInitial.SetActive(false);
            mutantIntermediate.SetActive(true);
        }

        if (other.CompareTag("GoldPlate") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldPlate.SetActive(false);
            durgaFlame1.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldCup1") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldCupOne.SetActive(false);
            durgaFlame2.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldCup2") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldCupTwo.SetActive(false);
            durgaFlame3.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldCup3") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldCupThree.SetActive(false);
            durgaFlame4.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldCandleStick") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldCandleStickOne.SetActive(false);
            durgaFlame5.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldCandleStick2") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldCandleStickTwo.SetActive(false);
            durgaFlame6.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldNecklace") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldNecklace.SetActive(false);
            durgaFlame7.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("GoldIngot") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            goldIngot.SetActive(false);
            durgaFlame8.SetActive(true);
            offerings++;

            if (offerings == 8)
            {
                remyAudio.PlayOneShot(rumbleSound);
                halfWall.SetActive(false);
            }
        }

        if (other.CompareTag("AdditionalTreasures") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldPouringSound, 2.0f);
            additionalTreasures.SetActive(false);
            halfWall.SetActive(true);

        }

        if (other.CompareTag("AdditionalTreasures") && Input.GetKey(KeyCode.Space) && mutantDead.activeInHierarchy)
        {
            remyAudio.PlayOneShot(goldPouringSound, 2.0f);
            additionalTreasures.SetActive(false);
            halfWall.SetActive(false);

        }

        if (other.CompareTag("Ruby") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            ruby.SetActive(false);
            halfWall.SetActive(true);

        }

        if (other.CompareTag("Ruby") && Input.GetKey(KeyCode.Space) && mutantDead.activeInHierarchy)
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            ruby.SetActive(false);
            halfWall.SetActive(false);

        }

        if (other.CompareTag("KeyPrompt") && Input.GetKey(KeyCode.Space))
        {
            keyPromptInstructions.SetActive(true);
            halfWall.SetActive(true);
            key.SetActive(true);
        }

        if (other.CompareTag("KeyPrompt") && Input.GetKey(KeyCode.Space) && mutantDead.activeInHierarchy)
        {
            keyPromptInstructions.SetActive(true);
            halfWall.SetActive(false);
            key.SetActive(false);
        }

        if (other.CompareTag("Key") && Input.GetKey(KeyCode.Space))
        {
            remyAudio.PlayOneShot(goldSound, 2.0f);
            key.SetActive(false);
            halfWall.SetActive(true);
        }

        if (other.CompareTag("ClosedExitDoor") && Input.GetKey(KeyCode.Space) && !key.activeInHierarchy)
        {
            closedExitDoor.SetActive(false);
            openExitDoor.SetActive(true);
            mutantIntermediate.SetActive(true);
        }
    }

    public void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Exit2"))
        {

            if (!ruby.activeInHierarchy && mutantIntermediate.activeInHierarchy)
            {
                openExitDoor.SetActive(false);
                closedExitDoor.SetActive(true);

                mutantIntermediate.SetActive(false);
                mutantFinal.SetActive(true);
                remyAudio.PlayOneShot(jumpScareSound);
                StartCoroutine(waiterTen());
                SceneManager.LoadScene(gameOverScene);
            }

            if (ruby.activeInHierarchy && mutantIntermediate.activeInHierarchy)
            {
                openExitDoor.SetActive(false);
                closedExitDoor.SetActive(true);

                mutantIntermediate.SetActive(false);
                mutantDead.SetActive(true);
                remyAudio.PlayOneShot(DyingSound);
                StartCoroutine(waiterTen());
                halfWall.SetActive(false);

                closedExitDoor.SetActive(false);
                openExitDoor.SetActive(true);

                closedEntranceDoor.SetActive(false);
                openEntranceDoor.SetActive(true);
            }
        }
    }
}