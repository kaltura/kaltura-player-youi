package tv.youi.kalturaplayeryoui.model;

public class WrapperYouboraConfig {

    private String accountCode;

    private String username;

    private String userEmail;

    private String userType;        // any string - free / paid etc.

    private boolean pasrseManifest = false;

    private boolean pasrseCdnNode = false;

    private String houseHoldId;    // which device is used to play

    private boolean httpSecure = true; // youbora events will be sent via https

    private String appName = "";

    private String appReleaseVersion = "";

    private String extraParam1;

    private String extraParam2;

    public WrapperYouboraConfig setAccountCode(String accountCode) {
        this.accountCode = accountCode;
        return this;
    }

    public String getAccountCode() {
        return accountCode;
    }

    public String getUsername() {
        return username;
    }

    public String getUserEmail() {
        return userEmail;
    }

    public String getUserType() {
        return userType;
    }

    public boolean isPasrseManifest() {
        return pasrseManifest;
    }

    public boolean isPasrseCdnNode() {
        return pasrseCdnNode;
    }

    public String getHouseHoldId() {
        return houseHoldId;
    }

    public boolean isHttpSecure() {
        return httpSecure;
    }

    public String getAppName() {
        return appName;
    }

    public String getAppReleaseVersion() {
        return appReleaseVersion;
    }

    public String getExtraParam1() {
        return extraParam1;
    }

    public String getExtraParam2() {
        return extraParam2;
    }
}

